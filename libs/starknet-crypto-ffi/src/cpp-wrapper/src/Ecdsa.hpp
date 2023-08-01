#pragma once

#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto/ecdsa.h>

namespace StarkwareCppWrapper
{

extern "C" {

int32_t ecdsa_sign( const uint64_t*, size_t, const uint64_t*, size_t, uint64_t*, uint64_t* );
int32_t sign( const uint64_t*, size_t, const uint64_t*, size_t, const uint64_t*, size_t, uint64_t*, uint64_t* );
int32_t ecdsa_verify(const uint64_t*, size_t, const uint64_t*, size_t, const uint64_t*, size_t, const uint64_t*, size_t, bool *);

}

class Ecdsa
{
  public:
    static starkware::Signature ecdsaSign(const starkware::PrimeFieldElement& privateKey, const starkware::PrimeFieldElement& messageHash);
    static starkware::Signature ecdsaSign(const starkware::PrimeFieldElement& privateKey, const starkware::PrimeFieldElement& messageHash, const starkware::PrimeFieldElement& k);

    static bool ecdsaVerify(const starkware::PrimeFieldElement& publicKey, const starkware::PrimeFieldElement& messageHash, const starkware::Signature& signature);
};


}