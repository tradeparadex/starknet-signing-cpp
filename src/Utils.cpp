#include <numeric>
#include "Utils.hpp"
#include "UtilsImpl.hpp"

#include "keccak.hpp"

namespace signer
{

PrimeFieldElement hashElements( const std::vector< PrimeFieldElement >& elements )
{
    constexpr PrimeFieldElement numInitValue = PrimeFieldElement::Zero();

    auto hash = []( const PrimeFieldElement& a, const PrimeFieldElement& b ) {
        return PedersenHash( a, b );
    };

    const PrimeFieldElement a = std::accumulate( elements.begin(), elements.end(), numInitValue,  hash);
    return hash(a, PrimeFieldElement::FromUint(elements.size()));
}

BigInt< 4 > getSelectorFromName( const char* name, size_t len )
{
    constexpr BigInt< 4 > numMask = 0x3ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_Z;

    const keccak256 hash = ethash_keccak256( (const uint8_t*)name, len );
    const BigInt< 4 > keccakHashBigInt( signer::to_array( hash.word64s ) );
    const auto reversed = signer::swapEndian( keccakHashBigInt );

    return numMask & reversed;
}

}
