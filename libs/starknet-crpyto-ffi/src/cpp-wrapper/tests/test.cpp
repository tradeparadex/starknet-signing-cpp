#include <starkware/algebra/prime_field_element.h>

#include "Ecdsa.hpp"

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

int main()
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    int code = 0;

    BigInt privateKey = 0x3c1e9550e66958296d11b60f8e8e7a7ad990d07fa65d5f7652c4a6c87d4e3cc_Z;
    for( int i = 0; i < 100; i++ )
    {
        privateKey = privateKey + 0x2_Z;
        code += rawBenchSignK(privateKey);
    }

    return code;
}