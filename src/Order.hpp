#pragma once

#include <optional>
#include <chrono>
#include <string>

#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto/ecdsa.h>

#include "HashableIface.hpp"

namespace signer
{

enum OrderSide
{
    Buy,
    Sell
};

static std::string toString( OrderSide side );
static starkware::PrimeFieldElement encodeChainSide( OrderSide value );

enum OrderType
{
    Market,
    Limit
};

std::string toString( OrderType type );
starkware::PrimeFieldElement encodeOrderType( OrderType value );

class Order: public HashableIface
{
  public:
    using Uint256 = starkware::PrimeFieldElement::ValueType;

    // TODO: fix strToBigInt constexpr issue with always returning BigInt<4>
    Order( const std::string& theMarket, OrderSide theOrderSide, OrderType theOrderType, double theSize,
        const std::optional< Uint256 >& theLimitPrice = std::nullopt, const std::string& theClientId = "" );

    Uint256 getChainPrice() const;

    void setTimestamp( std::chrono::milliseconds value );
    std::chrono::milliseconds getTimeStamp() const;

    std::string compileOrderRequestPayload( const starkware::Signature& signature ) const;
    virtual std::vector< starkware::PrimeFieldElement > pedersenEncode() const override;

  private:
    std::string clientId;
    std::string market;
    OrderSide orderSide;
    OrderType orderType;
    double size;
    std::chrono::milliseconds timestamp;
    std::optional< Uint256 > limitPrice;
};

} // signer
