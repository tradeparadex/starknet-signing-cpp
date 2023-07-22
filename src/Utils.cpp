#include <numeric>
#include <iostream>
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

    const PrimeFieldElement a = std::accumulate( elements.begin(), elements.end(), numInitValue, hash );
    return hash( a, PrimeFieldElement::FromUint( elements.size() ) );
}

BigInt< 4 > getSelectorFromName( const char* name, size_t len )
{
    constexpr BigInt< 4 > numMask = 0x3ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_Z;

    const keccak256 hash = ethash_keccak256( (const uint8_t*)name, len );
    const BigInt< 4 > keccakHashBigInt( signer::to_array( hash.word64s ) );
    const auto reversed = signer::swapEndian( keccakHashBigInt );

    return numMask & reversed;
}

PrimeFieldElement strToFelt( const char* str, size_t len )
{
    std::array< uint64_t, 4 > limbs = { 0, 0, 0, 0 };
    strToUint64ArrayImpl( str, len, limbs.data(), limbs.size() );

    const auto feltBig = BigInt( limbs );
    const auto feltBigR = BigInt( limbs );

    std::cout << "feltBig: "<< feltBig << std::endl;
    std::cout << "feltBigR: "<< swapEndian(feltBig) << std::endl;

    return PrimeFieldElement::FromBigInt( feltBig );
}

void strToUint64ArrayImpl( const char* src, size_t srcLen, uint64_t* dest, size_t destLen )
{
    constexpr uint8_t numIterations = sizeof( uint64_t ) / sizeof( char );

    int counter = 0;
    for( int i = 0; i >= 0; --i )
    {
        int charCounter = 0;
        while( counter < srcLen && charCounter < numIterations )
        {
            dest[ i ] <<= sizeof( char ) * 8;
            dest[ i ] |= ( uint64_t ) * ( src + counter );

            ++charCounter;
            ++counter;
        }
    }
}

} //
