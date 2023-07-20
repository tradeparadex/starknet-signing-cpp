#pragma once

#include <array>
#include <vector>
#include <algorithm>

#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto/pedersen_hash.h>
#include "hash_types.hpp"

namespace signer
{

using namespace starkware;

/*
Computes a hash chain over the data, in the following order:
    h(h(h(h(0, data[0]), data[1]), ...), data[n-1]), n).

The hash is initialized with 0 and ends with the data length appended.
The length is appended in order to avoid collisions of the following kind:
H([x,y,z]) = h(h(x,y),z) = H([w, z]) where w = h(x,y).
*/
PrimeFieldElement hashElements( const std::vector< PrimeFieldElement >& elements );

BigInt<4> getSelectorFromName( const char* name, size_t len );

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

template <typename T>
constexpr T swapEndian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

template<size_t N>
constexpr BigInt<N> swapEndian(const BigInt<N>& val)
{
    std::array<uint64_t, N> arr;
    for (uint i = 0; i < N; ++i)
    {
        arr[ N - i - 1 ] = swapEndian( val[ i ] );
    }

    return BigInt<N>(arr);
}

template<size_t N, size_t M>
BigInt<std::min(N, M)> operator&(const BigInt<N>& a, const BigInt<M>& b)
{
    constexpr size_t numMinLimbs = std::min( N, M );

    std::array< uint64_t, numMinLimbs > res;
    for( size_t i = 0; i < numMinLimbs; ++i )
    {
        res[ i ] = a[ i ] & b[ i ];
    }

    return BigInt< numMinLimbs >( res );
}

}
