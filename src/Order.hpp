#pragma once

#include <optional>
#include <chrono>
#include <string>

#include <starkware/algebra/prime_field_element.h>

namespace signer
{

enum OrderSide
{
    Buy,
    Sell
};

starkware::PrimeFieldElement encodeChainSide( OrderSide value );

enum OrderType
{
    Market,
    Limit
};

starkware::PrimeFieldElement encodeOrderType( OrderType value );

class Order
{
  public:
    using Uint256 = starkware::PrimeFieldElement::ValueType;

    // TODO: fix strToBigInt constexpr issue with always returning BigInt<4>
    Order( const std::string& theMarket, OrderSide theOrderSide, OrderType theOrderType, double theSize,
        const std::optional< Uint256 >& theLimitPrice = std::nullopt);

    std::vector< starkware::PrimeFieldElement > pedersenEncode() const;

    Uint256 getChainPrice() const;

  private:
    std::chrono::milliseconds timestamp;
    std::string market;
    OrderSide orderSide;
    OrderType orderType;
    double size;
    std::optional< Uint256 > limitPrice;
};

} // signer
