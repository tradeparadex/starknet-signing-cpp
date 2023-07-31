#include <iostream>

#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto//elliptic_curve_constants.h>
#include "starkware/crypto/pedersen_hash.h"

#include "Ecdsa.hpp"
#include "PedersenHash.hpp"

#define BENCHMARK_FUNCTION(func, ...)                             \
    auto start_time = std::chrono::high_resolution_clock::now(); \
    auto funcRes = func(__VA_ARGS__);                                        \
    auto end_time = std::chrono::high_resolution_clock::now();   \
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count(); \
    std::cout << "Execution time: " << duration << " microseconds" << std::endl; \


inline int rawBenchSign(const starkware::PrimeFieldElement::ValueType& privateKey)
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    const auto privateKeyFelt = PrimeFieldElement::FromBigInt( privateKey );

    const BigInt hash = 0x02ff954a62a6191411aa051588c65a4ac37690ce781bb345bb886ac947630e5b_Z;

    const auto rawPrivateKey = privateKeyFelt.ToMont().ToLimbs();
    const auto rawMessageHash = hash.ToLimbs();

    std::array< uint64_t, 4 > r = { 0, 0, 0, 0 };
    std::array< uint64_t, 4 > s = { 0, 0, 0, 0 };

    BENCHMARK_FUNCTION( ecdsa_sign, rawPrivateKey.data(), 4, rawMessageHash.data(), 4, r.data(), s.data() );
    return funcRes;
}

inline int rawBenchSignK(const starkware::PrimeFieldElement::ValueType& privateKey)
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    const auto privateKeyFelt = PrimeFieldElement::FromBigInt( privateKey );
    const auto k = 0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z;
    const BigInt hash = 0x02ff954a62a6191411aa051588c65a4ac37690ce781bb345bb886ac947630e5b_Z;

    const auto rawPrivateKey = privateKeyFelt.ToMont().ToLimbs();
    const auto rawMessageHash = hash.ToLimbs();
    const auto rawK = k.ToLimbs();

    std::array< uint64_t, 4 > r = { 0, 0, 0, 0 };
    std::array< uint64_t, 4 > s = { 0, 0, 0, 0 };

    BENCHMARK_FUNCTION( sign, rawPrivateKey.data(), 4, rawMessageHash.data(), 4, rawK.data(), 4, r.data(), s.data() );
    return funcRes;
}

inline int benchSign(const starkware::PrimeFieldElement::ValueType& privateKey)
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    const auto privateKeyFelt = PrimeFieldElement::FromBigInt( privateKey );
    const PrimeFieldElement hash = PrimeFieldElement::FromBigInt(0x02ff954a62a6191411aa051588c65a4ac37690ce781bb345bb886ac947630e5b_Z);

    BENCHMARK_FUNCTION( Ecdsa::ecdsaSign, privateKeyFelt, hash );
    return 0;
}

inline int benchSignK(const starkware::PrimeFieldElement::ValueType& privateKey)
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    const PrimeFieldElement privateKeyFelt = PrimeFieldElement::FromBigInt( privateKey );
    const PrimeFieldElement hash = PrimeFieldElement::FromBigInt(0x02ff954a62a6191411aa051588c65a4ac37690ce781bb345bb886ac947630e5b_Z);
    const PrimeFieldElement k = PrimeFieldElement::FromBigInt(0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z);

    BENCHMARK_FUNCTION( Ecdsa::ecdsaSign, privateKeyFelt, hash, k );
    return 0;
}

// C++ ~ verify  2x times slower
inline int testSignAndVerify(const starkware::PrimeFieldElement::ValueType& privateKey)
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    const auto publicKey = GetPublicKey( privateKey );
    const auto privateKeyFelt = PrimeFieldElement::FromBigInt( privateKey );
    const PrimeFieldElement hash = PrimeFieldElement::FromBigInt( 0x02ff954a62a6191411aa051588c65a4ac37690ce781bb345bb886ac947630e5b_Z );

    const Signature signature = Ecdsa::ecdsaSign( privateKeyFelt, hash );
    BENCHMARK_FUNCTION( Ecdsa::ecdsaVerify, publicKey.x, hash, signature );

    std::string s = funcRes ? "true" : "false";
    std::cout << "valid: " << s << std::endl;
}

inline int testSignAndVerifyCpp(const starkware::PrimeFieldElement::ValueType& privateKey)
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    const auto publicKey = GetPublicKey( privateKey );
    const auto privateKeyFelt = PrimeFieldElement::FromBigInt( privateKey );
    const PrimeFieldElement hash = PrimeFieldElement::FromBigInt( 0x02ff954a62a6191411aa051588c65a4ac37690ce781bb345bb886ac947630e5b_Z );

    const Signature signature = Ecdsa::ecdsaSign( privateKeyFelt, hash );
    const auto sFelt = PrimeFieldElement::FromBigInt( signature.second.ToStandardForm().InvModPrime( GetEcConstants().k_order ) );
    BENCHMARK_FUNCTION( VerifyEcdsa, publicKey, hash, { signature.first, sFelt } );

    std::string s = funcRes ? "true" : "false";
    std::cout << "valid: " << s << std::endl;
}


int nativePedersenHash()
{
    using namespace starkware;

    const auto x = PrimeFieldElement::FromBigInt(
        0x3d937c035c878245caf64531a5756109c53068da139362728feb561405371cb_Z);
    const auto y = PrimeFieldElement::FromBigInt(
        0x208a0a10250e382e1e4bbe2880906c2791bf6275695e02fbbc6aeff9cd8b31a_Z);

    BENCHMARK_FUNCTION(PedersenHash, x, y);
    std::cout << funcRes << std::endl;
}

int rsPedersenHash()
{
    using namespace starkware;

    const auto x = PrimeFieldElement::FromBigInt(
        0x3d937c035c878245caf64531a5756109c53068da139362728feb561405371cb_Z);
    const auto y = PrimeFieldElement::FromBigInt(
        0x208a0a10250e382e1e4bbe2880906c2791bf6275695e02fbbc6aeff9cd8b31a_Z);

    BENCHMARK_FUNCTION(StarkwareCppWrapper::PedersenHash::pedersenHash, x, y);
    std::cout << funcRes << std::endl;
}

int main()
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    int code = 0;

//    for( int i = 0; i < 100; i++ )
//    {
//        nativePedersenHash();
//    }

    BigInt privateKey = 0x3c1e9550e66958296d11b60f8e8e7a7ad990d07fa65d5f7652c4a6c87d4eccc_Z;
    for( int i = 0; i < 100; i++ )
    {
        privateKey = privateKey + 0x2_Z;
        code += benchSign(privateKey);
    }

    return code;
}
