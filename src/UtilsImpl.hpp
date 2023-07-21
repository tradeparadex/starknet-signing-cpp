#pragma once

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
    for( uint i = 0; i < N; ++i )
    {
        arr[ N - i - 1 ] = swapEndian( val[ i ] );
    }

    return BigInt< N >( arr );
}

template< size_t N, size_t M >
BigInt< std::min( N, M ) > operator&( const BigInt< N >& a, const BigInt< M >& b )
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
constexpr BigInt< StrToUint64Len< StrLen >::value > charToBigInt( const char* str )
{
    constexpr size_t numLimbsCount = StrToUint64Len< StrLen >::value;
    // uint64_t* castedStr = (uint64_t*) str;
    constexpr uint64_t arr[ numLimbsCount ] = (uint64_t*)str;

    const auto asd = to_array< uint64_t, numLimbsCount >( arr );

    return BigInt< numLimbsCount >( asd );
}

template< size_t N >
constexpr BigInt< 4 > bigIntToUint256( const BigInt< N >& value )
{
    constexpr size_t numUint256Limbs = 4;
    constexpr std::array< uint64_t, numUint256Limbs > limbs = { 0, 0, 0, 0 };

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
    const uint64_t* limbs = bigIntToArray( value ).data();
    return getSelectorFromName( (const char*)limbs, N * sizeof( uint64_t ) );
}

template< size_t StrLen >
std::array< uint64_t, StrToUint64Len< StrLen >::value > strToUint64Array( const char* str )
{
    constexpr size_t len64 = StrToUint64Len< StrLen >::value;
    constexpr uint8_t numIterations = sizeof( uint64_t ) / sizeof( char );

    std::array< uint64_t, len64 > arr = { { 0 } };
    std::fill( arr.begin(), arr.end(), 0 );

    int counter = 0;
    for( int i = len64 - 1; i >= 0; --i )
    {
        int charCounter = 0;
        while( counter < StrLen && charCounter < numIterations )
        {
            arr[ i ] <<= sizeof( char ) * 8;
            arr[ i ] |= ( uint64_t ) * ( str + counter );

            ++charCounter;
            ++counter;
        }
    }

    return arr;
}

} // signer