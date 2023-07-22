#pragma once

#include "EncodableIface.hpp"

#include <starkware/algebra/prime_field_element.h>

namespace signer
{

/// To Hash smth this value has to be Encodable into Felt
class HashableIface: public EncodableIface
{
  public:
    using Pointer = std::shared_ptr<HashableIface>;

    virtual starkware::PrimeFieldElement hash() const;
};

}
