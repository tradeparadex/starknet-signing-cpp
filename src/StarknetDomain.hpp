#pragma once
#include <string>

#include "HashableIface.hpp"

#include <starkware/algebra/prime_field_element.h>

namespace signer
{

class StarknetDomain: public HashableIface
{
  public:
    using Uint256 = starkware::PrimeFieldElement::ValueType;

    explicit StarknetDomain( const Uint256& chainId );
    explicit StarknetDomain( const starkware::PrimeFieldElement& chainId );

    virtual std::vector< starkware::PrimeFieldElement > pedersenEncode() const override;

  private:
    // Those types are contant for Order & Auth
    static constexpr Uint256 numVersionId = Uint256::One();
    Uint256 chainId = Uint256::Zero();
};

}
