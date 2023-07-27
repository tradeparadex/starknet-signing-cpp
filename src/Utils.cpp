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

BigInt< 4 > strToBigInt( const char* str)
{
    std::array< uint64_t, 4 > limbs = { 0, 0, 0, 0 };
    strToUint64ArrayImpl( str, strlen( str ), limbs.data(), limbs.size() );

    return BigInt( limbs );
}

PrimeFieldElement strToFelt( const char* str, size_t len )
{
    std::array< uint64_t, 4 > limbs = { 0, 0, 0, 0 };
    strToUint64ArrayImpl( str, len, limbs.data(), limbs.size() );

    return PrimeFieldElement::FromBigInt( BigInt( limbs ) );
}

void strToUint64ArrayImpl( const char* src, size_t srcLen, uint64_t* dest, size_t destLen )
{
    static_assert( CHAR_BIT == 8, "CHAR_BIT != 8" );

    constexpr uint8_t numIterations = sizeof( uint64_t ) / sizeof( char );

    size_t len = srcLen;
    for( uint i = 0; i < destLen; i++ )
    {
        uint8_t charCounter = 0;
        while( len != 0 && charCounter != numIterations )
        {
            const uint64_t currentChar = (uint64_t)*(src + len - 1);
            dest[ i ] |= currentChar << CHAR_BIT * charCounter;

            charCounter++;
            len--;
        }
    }
}

constexpr void swapEndian( uint64_t* arr, size_t len )
{
    if( len == 0 )
    {
        return;
    }

    size_t start = 0;
    size_t end = len - 1;

    while( start < end )
    {
        uint64_t temp = arr[ start ];
        arr[ start ] = swapEndian( arr[ end ] );
        arr[ end ] = swapEndian( temp );

        start++;
        end--;
    }
}

} // signer
