#include <iostream>
#include <array>
#include <algorithm>
#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto//elliptic_curve_constants.h>

#include "Utils.hpp"
#include "TestUtils.hpp"
#include "StarkCurveSigner.hpp"
#include "Order.hpp"
#include "Message.hpp"

#define BENCHMARK_FUNCTION(func, ...)                             \
    auto start_time = std::chrono::high_resolution_clock::now(); \
    func(__VA_ARGS__);                                        \
    auto end_time = std::chrono::high_resolution_clock::now();   \
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count(); \
    std::cout << "Execution time: " << duration << " microseconds" << std::endl; \

extern "C" {
int32_t ecdsa_sign( const uint64_t*, size_t, const uint64_t*, size_t, uint64_t*, uint64_t* );
}

std::array<uint64_t, 4> feltToArray(const starkware::PrimeFieldElement& value)
{
    std::array<uint64_t, 4> output;
    const auto bigInt = value.ToStandardForm();
    for (int i = 0; i<4;i++)
    {
        output[i] = bigInt[i];
    }

    return output;
}

std::array<uint64_t, 4> bigIntToArray(const starkware::PrimeFieldElement::ValueType& value)
{
    std::array<uint64_t, 4> output;
    for (int i = 0; i<4;i++)
    {
        output[i] = value[i];
    }

    return output;
}

inline void benchSign(const starkware::PrimeFieldElement::ValueType& privateKey)
{
    using namespace signer;
    using namespace starkware;

    const auto privateKeyFelt = PrimeFieldElement::FromBigInt( privateKey );

    KeyPair keyPair( privateKey );
    StarkCurveSigner signer( keyPair );

    Message message = getOrderMessage();
    const auto hash = message.hash();

    const auto private_key = privateKeyFelt.ToMont().ToLimbs();
    const auto message_hash = hash.ToMont().ToLimbs();
    std::array< uint64_t, 4 > r = { 0, 0, 0, 0 };
    std::array< uint64_t, 4 > s = { 0, 0, 0, 0 };

    BENCHMARK_FUNCTION( ecdsa_sign, private_key.data(), 4, message_hash.data(), 4, r.data(), s.data() );
}

int testCorrectness(const starkware::PrimeFieldElement::ValueType& privateKey)
{
    using namespace signer;
    using namespace starkware;

    const auto privateKeyFelt = PrimeFieldElement::FromBigInt( privateKey );

    KeyPair keyPair( privateKey );
    StarkCurveSigner signer( keyPair );

    Message message = getOrderMessage();
    const auto hash = message.hash();

    const auto private_key = privateKeyFelt.ToMont().ToLimbs();
    const auto message_hash = hash.ToMont().ToLimbs();
    std::array< uint64_t, 4 > r = { 0, 0, 0, 0 };
    std::array< uint64_t, 4 > s = { 0, 0, 0, 0 };

    BENCHMARK_FUNCTION( ecdsa_sign, private_key.data(), 4, message_hash.data(), 4, r.data(), s.data() );

    const auto rFelt = PrimeFieldElement::FromMont( starkware::BigInt( r ) );
    const auto sInvFelt = PrimeFieldElement::FromMont( starkware::BigInt( s ) );
    const auto sFelt = PrimeFieldElement::FromBigInt( sInvFelt.ToStandardForm().InvModPrime( starkware::GetEcConstants().k_order ) );

    bool isValid = signer.verifyEcdsa( hash, { rFelt, sFelt } );
    return !isValid;
}

int main()
{
    using namespace signer;
    using namespace starkware;

    auto privateKey = 0x3c1e9550e66958296d11b60f8e8e7a7ad990d07fa65d5f7652c4a6c87d4e3cc_Z;
    for( int i = 0; i < 100; i++ )
    {
        privateKey = privateKey + 0x2_Z;
        benchSign(privateKey);
    }

    return testCorrectness(privateKey);
}
