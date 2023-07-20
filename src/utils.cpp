#include <numeric>
#include "utils.hpp"

#include "keccak.hpp"

namespace signer
{

PrimeFieldElement hashElements( const std::vector< PrimeFieldElement >& elements )
{
    constexpr PrimeFieldElement numInitValue = PrimeFieldElement::Zero();

    return std::accumulate( elements.begin(), elements.end(), numInitValue, []( const PrimeFieldElement& a, const PrimeFieldElement& b ) {
        return PedersenHash( a, b );
    } );
}

BigInt<4> getSelectorFromName(const char* name, size_t len)
{
    constexpr BigInt<4> numMask =  0x3ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_Z;

    const keccak256 hash = ethash_keccak256((const uint8_t*)name, len);
    const BigInt< 4 > keccakHashBigInt( signer::to_array( hash.word64s ) );
    const auto reversed = signer::swapEndian(keccakHashBigInt);

    return numMask & reversed;
}

}
