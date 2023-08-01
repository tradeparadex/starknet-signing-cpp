#pragma once

#include <starkware/algebra/prime_field_element.h>


namespace StarkwareCppWrapper
{

extern "C" {
int32_t pedersen_hash( const uint64_t*, size_t, const uint64_t*, size_t, uint64_t* );
}

class PedersenHash
{
  public:
    static starkware::PrimeFieldElement pedersenHash( const starkware::PrimeFieldElement& x, const starkware::PrimeFieldElement& y );
};

}
