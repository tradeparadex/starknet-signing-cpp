#pragma once

#include <optional>

#include <starkware/algebra/prime_field_element.h>

namespace signer
{

enum OrderSide
{
    Buy,
    Sell
};

starkware::PrimeFieldElement encodeChainSide(OrderSide value);

enum OrderType
{
    Market,
    Limit
};

starkware::PrimeFieldElement encodeOrderType(OrderType value);

class Order
{
  public:
    using Uint256 = starkware::PrimeFieldElement::ValueType;

    // TODO: fix strToBigInt constexpr issue with always returning BigInt<4>
    Order( const char* theMarket, OrderSide theOrderSide, OrderType theOrderType,
        double theSize, const std::optional<Uint256>& theLimitPrice );

    std::vector< starkware::PrimeFieldElement > pedersenEncode() const;

    Uint256 getChainPrice() const;

  private:
    Uint256 timestamp = Uint256::Zero();
    Uint256 market;
    OrderSide orderSide;
    OrderType orderType;
    double size;
    std::optional<Uint256> limitPrice;
};

} // signer
