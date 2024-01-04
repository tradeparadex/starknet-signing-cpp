#pragma once

#include <limits>
#include "keccak.hpp"

#include "Utils.hpp"

namespace signer
{

namespace
{

template< class T, std::size_t N, std::size_t... I >
constexpr std::array< std::remove_cv_t< T >, N > to_array_impl( T ( &a )[ N ], std::index_sequence< I... > )
{
    return { { a[ I ]... } };
}

}

template< class T, std::size_t N >
constexpr std::array< std::remove_cv_t< T >, N > to_array( T ( &a )[ N ] )
{
    return to_array_impl( a, std::make_index_sequence< N >{} );
}

template< typename T >
constexpr T swapEndian( T u )
{
    static_assert( CHAR_BIT == 8, "CHAR_BIT != 8" );

    union
    {
        T u;
        unsigned char u8[ sizeof( T ) ];
    } source, dest;

    source.u = u;

    for( size_t k = 0; k < sizeof( T ); k++ )
        dest.u8[ k ] = source.u8[ sizeof( T ) - k - 1 ];

    return dest.u;
}

template< size_t N >
constexpr BigInt< N > swapEndian( const BigInt< N >& val )
{
    std::array< uint64_t, N > arr;
    for( size_t i = 0; i < N; ++i )
    {
        arr[ N - i - 1 ] = swapEndian( val[ i ] );
    }

    return BigInt<N>(arr);
//
//    if ( N == 0 )
//    {
//        return;
//    }
//
//    int start = 0;
//    int end = N - 1;
//
//    while (start < end) {
//        int temp = arr[start];
//        arr[start] = swapEndian(arr[end]);
//        arr[end] = swapEndian(temp);
//
//        start++;
//        end--;
//    }
}

template< size_t N, size_t M >
BigInt< (std::min)( N, M ) > operator&( const BigInt< N >& a, const BigInt< M >& b )
{
    constexpr size_t numMinLimbs = std::min( N, M );

    std::array< uint64_t, numMinLimbs > res;
    for( size_t i = 0; i < numMinLimbs; ++i )
    {
        res[ i ] = a[ i ] & b[ i ];
    }

    return BigInt< numMinLimbs >( res );
}

template< size_t StrLen >
BigInt< StrToUint64Len< StrLen >::value > strToBigInt( const char* str )
{
    constexpr size_t numLimbsCount = StrToUint64Len< StrLen >::value;
    const auto uint64Array = strToUint64Array< StrLen >( str );

    return BigInt< numLimbsCount >( uint64Array );
}

template< size_t N >
BigInt< 4 > bigIntToUint256( const BigInt< N >& value )
{
    constexpr size_t numUint256Limbs = 4;
    std::array< uint64_t, numUint256Limbs > limbs = { 0, 0, 0, 0 };

    for( size_t i = 0; i < std::min( N, numUint256Limbs ); ++i )
    {
        limbs[ i ] = value[ i ];
    }

    return BigInt< 4 >( limbs );
}

template< size_t N >
constexpr std::array< uint64_t, N > bigIntToArray( const BigInt< N >& value )
{
    std::array< uint64_t, N > result = { { 0 } };
    for( size_t i = 0; i < N; i++ )
    {
        result[ i ] = value[ i ];
    }

    return result;
}

template< size_t N >
BigInt< 4 > getSelectorFromName( const BigInt< N >& value )
{
    constexpr BigInt< 4 > numMask = 0x3ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_Z;

    const std::array< uint64_t, N > bigIntArray = bigIntToArray( value );
    const uint64_t* limbs = bigIntArray.data();

    return getSelectorFromName( (const char*)limbs, N * sizeof( uint64_t ) );
}

template< size_t StrLen >
std::array< uint64_t, StrToUint64Len< StrLen >::value > strToUint64Array( const char* str )
{
    constexpr size_t len64 = StrToUint64Len< StrLen >::value;

    std::array< uint64_t, len64 > arr;
    std::fill( arr.begin(), arr.end(), 0 );

    strToUint64ArrayImpl(str, StrLen, arr.data(), len64);

    return arr;
}

/// Replaces token in string
template< class T >
void replace( std::string* header, const char* token, T val )
{
    constexpr uint8_t numTokenLen = 2;

    std::ostringstream s;
    s << val;

    // Find the position of "%S" in the original string
    size_t pos = header->find( token );

    // Check if "%S" was found in the string
    if( pos == std::string::npos )
    {
        throw new std::length_error( "Couldn't find %1" );
    }

    header->replace( pos, numTokenLen, s.str() );
}

} // namespace signer
