#include <numeric>
#include <iostream>
#include <cstring>
#include <limits>
#include "keccak.hpp"
#include "ApiException.hpp"

#include "Config.hpp"
#include "Utils.hpp"
#include "UtilsImpl.hpp"

#if ENABLE_CPP
#include <starkware/crypto/pedersen_hash.h>
#else
#include "PedersenHash.hpp"
#endif

namespace signer
{

PrimeFieldElement hashElements( const std::vector< PrimeFieldElement >& elements )
{
    constexpr PrimeFieldElement numInitValue = PrimeFieldElement::Zero();

    auto hash = []( const PrimeFieldElement& a, const PrimeFieldElement& b ) {
#if ENABLE_CPP
        return PedersenHash( a, b );
#else
        try
        {
            return StarkwareCppWrapper::PedersenHash::pedersenHash( a, b );
        }
        catch( StarkwareCppWrapper::ApiException& e )
        {
            std::cerr << e.what() << std::endl;
            throw e;
        }
#endif
    };

    const PrimeFieldElement a = std::accumulate( elements.begin(), elements.end(), numInitValue, hash );
    return hash( a, PrimeFieldElement::FromUint( elements.size() ) );
}

BigInt< 4 > getSelectorFromName( const char* name, size_t len )
{
    constexpr BigInt< 4 > numMask = 0x3ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_Z;

    const keccak256 hash = ethash_keccak256( (const uint8_t*)name, len );
    const BigInt< 4 > keccakHashBigInt( signer::to_array( hash.word64s ) );
    const BigInt< 4 > reversed = signer::swapEndian( keccakHashBigInt );

    return numMask & reversed;
}

BigInt< 4 > strToBigInt256( const char* str )
{
    std::array< uint64_t, 4 > limbs = { 0, 0, 0, 0 };
    strToUint64ArrayImpl( str, std::strlen( str ), limbs.data(), limbs.size() );

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
    for( uint32_t i = 0; i < destLen; i++ )
    {
        uint8_t charCounter = 0;
        while( len != 0 && charCounter != numIterations )
        {
            const uint64_t currentChar = ( uint64_t ) * ( src + len - 1 );
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

/// Removes leading zeros in hex
void removeLeadingZeroes( std::string* value )
{
    std::string& hexValue = *value;

    // Check if the first 2 characters are "0x"
    if( hexValue.length() < 2 || hexValue.substr( 0, 2 ) != "0x" )
    {
        return;
    }

    size_t startPos = 2;

    // Find the position of the first non-zero character after "0x"
    while( startPos < hexValue.length() && hexValue[ startPos ] == '0' )
    {
        startPos++;
    }

    // Extract the substring starting from the first non-zero character
    if( startPos < hexValue.length() )
    {
        hexValue = "0x" + hexValue.substr( startPos );
    }
    else
    {
        // If the entire string is zeros, keep at least one zero.
        hexValue = "0x0";
    }
}

} // namespace signer
