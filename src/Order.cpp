#include <chrono>
#include <exception>
#include <<stdexcept>>

#include "Order.hpp"
#include "UtilsImpl.hpp"

namespace signer
{

starkware::PrimeFieldElement encodeChainSide( OrderSide value )
{
    using namespace starkware;
    using Uint256 = PrimeFieldElement::ValueType;

    switch( value )
    {
        case OrderSide::Buy:
            return PrimeFieldElement::FromBigInt( Uint256( 1 ) );
        case OrderSide::Sell:
            return PrimeFieldElement::FromBigInt( Uint256( 2 ) );
    }
}

starkware::PrimeFieldElement encodeOrderType( OrderType value )
{
    using namespace starkware;

    static constexpr char const* strMarket = "MARKET";
    static constexpr char const* strLimit = "LIMIT";

    switch( value )
    {
        case OrderType::Market:
        {
            constexpr size_t numMarketLen = std::char_traits< char >::length( strMarket );
            const auto tmpMarket = signer::strToBigInt< numMarketLen >( strMarket );
            const auto market = signer::bigIntToUint256( tmpMarket );

            return PrimeFieldElement::FromBigInt( market );
        }
        case OrderType::Limit:
        {
            constexpr size_t numLimitLen = std::char_traits< char >::length( strLimit );
            const auto tmpLimit = signer::strToBigInt< numLimitLen >( strLimit );
            const auto limit = signer::bigIntToUint256( tmpLimit );

            return PrimeFieldElement::FromBigInt( limit );
        }
    }
}

Order::Order( const Uint256& theMarket, OrderSide theOrderSide, OrderType theOrderType, double theSize, const std::optional<Uint256>& theLimitPrice )
    : market( theMarket )
    , orderSide( theOrderSide )
    , orderType( theOrderType )
    , size( theSize )
{
    using namespace std::chrono;

    milliseconds ms = duration_cast< milliseconds >( system_clock::now().time_since_epoch() );

    timestamp = Uint256( ms.count() );

    if (orderType == OrderType::Limit && !theLimitPrice.has_value())
    {
        throw new std::invalid_argument("If OrderType::Limit theLimitPrice shall exist");
    }

    limitPrice = theLimitPrice;
}

Order::Uint256 Order::getChainPrice() const
{
    if(orderType == OrderType::Market)
    {
        return Uint256 (0);
    }

    return limitPrice.value();
}


std::vector< starkware::PrimeFieldElement > Order::pedersenEncode() const
{
    using namespace starkware;

    const PrimeFieldElement timestamp =  PrimeFieldElement::FromBigInt(this->timestamp);
    const PrimeFieldElement chainSide = encodeChainSide( this->orderSide );
    const PrimeFieldElement orderType = signer::encodeOrderType(this->orderType);
    const PrimeFieldElement chainPrice = PrimeFieldElement::FromBigInt(getChainPrice());
    const PrimeFieldElement chainSize = PrimeFieldElement::FromUint(size * 100000000);

    const tmp
}

} // namespace signer