#include <iostream>
#include "gtest/gtest.h"
#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto//elliptic_curve_constants.h>

#include "Ecdsa.hpp"
#include "PedersenHash.hpp"

#define BENCHMARK_FUNCTION(func, ...)                             \
    auto start_time = std::chrono::high_resolution_clock::now(); \
    auto funcRes = func(__VA_ARGS__);                                        \
    auto end_time = std::chrono::high_resolution_clock::now();   \
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count(); \
    std::cout << "Execution time: " << duration << " microseconds" << std::endl; \

TEST(StarkwareCppWrapper, ecdsaSign)
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    const PrimeFieldElement privateKeyFelt = PrimeFieldElement::FromBigInt( 0x0139fe4d6f02e666e86a6f58e65060f115cd3c185bd9e98bd829636931458f99_Z );
    const PrimeFieldElement hash = PrimeFieldElement::FromBigInt( 0x06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76_Z );

    BENCHMARK_FUNCTION( Ecdsa::ecdsaSign, privateKeyFelt, hash );

    EXPECT_EQ(funcRes.first, PrimeFieldElement::FromBigInt(0x00798045aea59f67ac05a0aaedbcaf7f55156b9d0cd152fc91b6649a5fba8c13_Z));
    EXPECT_EQ(funcRes.second, PrimeFieldElement::FromBigInt(0x034a4770ff3bcf9d9a923a26232ae134b543cf6885768ac9c42e55fc79e531c4_Z));
}

TEST(StarkwareCppWrapper, ecdsaSignAndVerify)
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    const PrimeFieldElement privateKeyFelt = PrimeFieldElement::FromBigInt( 0x1_Z );
    const PrimeFieldElement hash = PrimeFieldElement::FromBigInt( 0x2_Z );

    const Signature signature = Ecdsa::ecdsaSign( privateKeyFelt, hash );

    const auto publicKey = GetPublicKey( privateKeyFelt.ToStandardForm() );
    BENCHMARK_FUNCTION( Ecdsa::ecdsaVerify, publicKey.x, hash, signature );
    EXPECT_TRUE( funcRes );
}

TEST(StarkwareCppWrapper, signRSVerifyCPP)
{
    using namespace starkware;
    using namespace StarkwareCppWrapper;

    const PrimeFieldElement hash = PrimeFieldElement::FromBigInt( 0x02ff954a62a6191411aa051588c65a4ac37690ce781bb345bb886ac947630e5b_Z );
    const PrimeFieldElement privateKeyFelt = PrimeFieldElement::FromBigInt( 0x0139fe4d6f02e666e86a6f58e65060f115cd3c185bd9e98bd829636931458f99_Z );
    const auto publicKey = GetPublicKey( privateKeyFelt.ToStandardForm() );

    // Get signature from rust
    const Signature signature = Ecdsa::ecdsaSign( privateKeyFelt, hash );

    // Inverse s before verifying in cpp
    const PrimeFieldElement sFelt = PrimeFieldElement::FromBigInt( signature.second.ToStandardForm().InvModPrime( GetEcConstants().k_order ) );
    BENCHMARK_FUNCTION( VerifyEcdsa, publicKey, hash, { signature.first, sFelt } );

    EXPECT_TRUE(funcRes);
}

TEST(StarkwareCppWrapper, pedersenHash)
{
    using namespace starkware;
    const PrimeFieldElement x = PrimeFieldElement::FromBigInt(
        0x3d937c035c878245caf64531a5756109c53068da139362728feb561405371cb_Z);
    const PrimeFieldElement y = PrimeFieldElement::FromBigInt(
        0x208a0a10250e382e1e4bbe2880906c2791bf6275695e02fbbc6aeff9cd8b31a_Z);

    BENCHMARK_FUNCTION(StarkwareCppWrapper::PedersenHash::pedersenHash, x, y);
}
