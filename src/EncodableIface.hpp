#pragma once

#include <vector>
#include <memory>

#include "starkware/algebra/prime_field_element.h"

namespace signer
{

class EncodableIface
{
  public:
    using Pointer = std::shared_ptr<EncodableIface>;

    // Encoding only suitable for Pedersen hash.
    virtual std::vector< starkware::PrimeFieldElement > pedersenEncode() const = 0;
};

}