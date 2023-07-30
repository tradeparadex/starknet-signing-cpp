#include <iostream>
#include <array>
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

//starkware::PrimeFieldElement pointerToFelt(const uint64_t* value)
//{
//    starkware::BigInt<4> output = starkware::BigInt<4>::Zero();
//
//    for(int i = 0; i < 4; i++)
//    {
//        output[i] = value[i];
//    }
//
//    return starkware::PrimeFieldElement::FromBigInt(output);
//}


#include <string>
#include <algorithm>

// Function to swap the endianness of a hexadecimal string
std::string swapEndianness(const std::string& hexString) {
    std::string swappedString;

    // Iterate through the hexadecimal string in pairs and reverse the byte order
    for (size_t i = 0; i < hexString.size(); i += 2) {
        swappedString.insert(0, hexString.substr(i, 2));
    }

    return swappedString;
}

int main()
{
    using namespace signer;
    using namespace starkware;

//    Prng prng;
//    using ValueType = PrimeFieldElement::ValueType;
//
//    // Draw test parameters.
//    //const auto privateKey = ValueType::RandomBigInt( &prng );

    const auto privateKey = 0x3c1e9550e66958296d11b60f8e8e7a7ad990d07fa65d5f7652c4a6c87d4e3cc_Z;
    const auto privateKeyFelt = PrimeFieldElement::FromBigInt(privateKey);

    KeyPair keyPair( privateKey );
    StarkCurveSigner signer( keyPair );

    Message message = getOrderMessage();
    const auto hash = message.hash();
    {
        const auto private_key = privateKeyFelt.ToMont().ToLimbs();
        const auto message_hash =  hash.ToMont().ToLimbs();
        std::array< uint64_t, 4 > r = { 0, 0, 0, 0 };
        std::array< uint64_t, 4 > s = { 0, 0, 0, 0 };

        int res = ecdsa_sign( private_key.data(), 4, message_hash.data(), 4, r.data(), s.data() );

        const auto rFelt = PrimeFieldElement::FromMont(starkware::BigInt( r ) );
        const auto sInvFelt = PrimeFieldElement::FromMont(starkware::BigInt( s ));
        const auto sFelt = PrimeFieldElement::FromBigInt( sInvFelt.ToStandardForm().InvModPrime( starkware::GetEcConstants().k_order ) );

        bool isValid = signer.verifyEcdsa( hash, { rFelt, sFelt } );
        return res || !isValid;
    }
}
