#include <starkware/algebra/big_int.h>

#include "Ecdsa.hpp"

namespace StarkwareCppWrapper
{

starkware::Signature Ecdsa::ecdsaSign( const starkware::PrimeFieldElement& privateKey, const starkware::PrimeFieldElement& messageHash )
{
    using namespace starkware;

    // TODO: replace 4 with const
    const std::array< uint64_t, 4 > rawPrivateKey = privateKey.ToMont().ToLimbs();
    const std::array< uint64_t, 4 > rawMessageHash = messageHash.ToMont().ToLimbs();

    std::array< uint64_t, 4 > rawR = { 0, 0, 0, 0 };
    std::array< uint64_t, 4 > rawS = { 0, 0, 0, 0 };

    /// NOTE: s is inversed
    /// TODO: process code
    int code = ecdsa_sign(rawPrivateKey.data(), 4, rawMessageHash.data(), 4, rawR.data(), rawS.data());

    const PrimeFieldElement r = PrimeFieldElement::FromMont( BigInt( rawR ) );
    const PrimeFieldElement s = PrimeFieldElement::FromMont( BigInt(rawS));

    return {r,s};
}

starkware::Signature Ecdsa::ecdsaSign(
    const starkware::PrimeFieldElement& privateKey, const starkware::PrimeFieldElement& messageHash, const starkware::PrimeFieldElement& k )
{
    using namespace starkware;

    // TODO: replace 4 with const
    const std::array< uint64_t, 4 > rawPrivateKey = privateKey.ToMont().ToLimbs();
    const std::array< uint64_t, 4 > rawMessageHash = messageHash.ToMont().ToLimbs();
    const std::array< uint64_t, 4 > rawK = k.ToMont().ToLimbs();

    std::array< uint64_t, 4 > rawR = { 0, 0, 0, 0 };
    std::array< uint64_t, 4 > rawS = { 0, 0, 0, 0 };

    /// NOTE: s is inversed
    /// TODO: process code
    int code = sign( rawPrivateKey.data(), 4, rawMessageHash.data(), 4, rawK.data(), 4, rawR.data(), rawS.data() );

    const PrimeFieldElement r = PrimeFieldElement::FromMont( BigInt( rawR ) );
    const PrimeFieldElement s = PrimeFieldElement::FromMont( BigInt( rawS ) );

    return { r, s };
}

}