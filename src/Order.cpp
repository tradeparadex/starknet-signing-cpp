#include <chrono>
#include <cstring>
#include <starkware/crypto/elliptic_curve_constants.h>
#include <nlohmann/json.hpp>

#include "Order.hpp"
#include "UtilsImpl.hpp"
#include "SignerException.hpp"

namespace signer
{
Order::Order( const std::string& theMarket, OrderSide theOrderSide, OrderType theOrderType, double theSize,
    const std::optional< Uint256 >& theLimitPrice /* = std::nullopt */, const std::string& theClientId /* = "" */ )
    : orderSide( theOrderSide )
    , market( theMarket )
    , orderType( theOrderType )
    , size( theSize )
    , clientId( theClientId )
{
    using namespace std::chrono;

    timestamp = duration_cast< milliseconds >( system_clock::now().time_since_epoch() );

    if( orderType == OrderType::Limit && !theLimitPrice.has_value() )
    {
        throw new SignerException( "If OrderType::Limit theLimitPrice shall exist" );
    }
    limitPrice = theLimitPrice;
}

// static
std::string toString( OrderSide side )
{
    switch( side )
    {
        case OrderSide::Buy:
            return "BUY";
        case OrderSide::Sell:
            return "SELL";
    }
}

// static
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

// static
std::string toString( OrderType side )
{
    switch( side )
    {
        case OrderType::Market:
            return "MARKET";
        case OrderType::Limit:
            return "LIMIT";
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

Order::Uint256 Order::getChainPrice() const
{
    if( orderType == OrderType::Market )
    {
        return Uint256( 0 );
    }

    return Uint256::MulMod(limitPrice.value(), 0x5f5e100_Z, GetEcConstants().k_order);
}

void Order::setTimestamp( std::chrono::milliseconds value )
{
    timestamp = value;
}

std::chrono::milliseconds Order::getTimeStamp() const
{
    return timestamp;
}

std::string Order::compileOrderRequestPayload( const starkware::Signature& signature ) const
{
    std::string signatureVal = "[\"%1\", \"%2\"]";
    replace( &signatureVal, "%1", signature.first );
    replace( &signatureVal, "%2", signature.second );

    nlohmann::json payload = { { "market", market }, { "side", toString( orderSide ) }, { "size", std::to_string( size ) },
        { "type", toString( orderType ) }, { "client_id", clientId }, { "signature", signatureVal },
        { "signature_timestamp", this->timestamp.count() } };

    if(orderType == OrderType::Limit)
    {
        payload["price"] = (*limitPrice).ToStringDec();
    }

    return payload.dump();
}

std::vector< starkware::PrimeFieldElement > Order::pedersenEncode() const
{
    using namespace starkware;

    const char* strOrderTypeName = "Order(timestamp:felt,market:felt,side:felt,orderType:felt,size:felt,price:felt)";

    const BigInt< 4 > tmpOrderTypeName = signer::getSelectorFromName( strOrderTypeName, std::strlen( strOrderTypeName ) );
    const PrimeFieldElement orderTypeName = PrimeFieldElement::FromBigInt( tmpOrderTypeName );
    const PrimeFieldElement timestamp = PrimeFieldElement::FromUint( this->timestamp.count() );
    const PrimeFieldElement market = signer::strToFelt( this->market.c_str(), this->market.length() );
    const PrimeFieldElement chainSide = encodeChainSide( this->orderSide );
    const PrimeFieldElement orderType = signer::encodeOrderType( this->orderType );
    const PrimeFieldElement chainSize = PrimeFieldElement::FromUint( size * 100000000 );
    const PrimeFieldElement chainPrice = PrimeFieldElement::FromBigInt( getChainPrice() );

    return { orderTypeName, timestamp, market, chainSide, orderType, chainSize, chainPrice };
}

} // namespace signer