#pragma once

#include <array>
#include <vector>
#include <algorithm>

#include <starkware/algebra/prime_field_element.h>
#include "hash_types.hpp"

namespace signer
{

using namespace starkware;

template< size_t StrLen >
struct StrToUint64Len
{
    static constexpr size_t value = ( sizeof( char ) * StrLen + sizeof( uint64_t ) - 1 ) / sizeof( uint64_t );
};

/*
Computes a hash chain over the data, in the following order:
    h(h(h(h(0, data[0]), data[1]), ...), data[n-1]), n).

The hash is initialized with 0 and ends with the data length appended.
The length is appended in order to avoid collisions of the following kind:
H([x,y,z]) = h(h(x,y),z) = H([w, z]) where w = h(x,y).
*/
PrimeFieldElement hashElements( const std::vector< PrimeFieldElement >& elements );

BigInt< 4 > getSelectorFromName( const char* name, size_t len );

template< size_t N >
BigInt< 4 > getSelectorFromName( const BigInt< N >& value );

template< size_t N >
BigInt< 4 > bigIntToUint256( const BigInt< N >& value );

template< class T, std::size_t N >
constexpr std::array< std::remove_cv_t< T >, N > to_array( T ( &a )[ N ] );

template< typename T >
constexpr T swapEndian( T u );

template< size_t N >
constexpr BigInt< N > swapEndian( const BigInt< N >& val );

constexpr void swapEndian( uint64_t* dest, size_t destLen );

template< size_t N, size_t M >
BigInt< (std::min)( N, M ) > operator&( const BigInt< N >& a, const BigInt< M >& b );

template< size_t N >
constexpr std::array< uint64_t, N > bigIntToArray( const BigInt< N >& value );

template< size_t StrLen >
BigInt< StrToUint64Len< StrLen >::value > strToBigInt( const char* str );

BigInt< 4 > strToBigInt256( const char* str );
PrimeFieldElement strToFelt( const char* str, size_t len );

/// string shall be supplied in ascii encoding
template< size_t StrLen >
std::array< uint64_t, StrToUint64Len< StrLen >::value > strToUint64Array( const char* str );

void strToUint64ArrayImpl( const char* src, size_t srcLen, uint64_t* dest, size_t destLen );

template< class T >
void replace( std::string* header, const char* token, T val );

void removeLeadingZeroes( std::string* value );

} // namespace signer
