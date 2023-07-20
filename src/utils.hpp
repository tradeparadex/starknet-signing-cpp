#pragma once

#include <initializer_list>

#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto/pedersen_hash.h>

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
PrimeFieldElement hashElements( const std::initializer_list< PrimeFieldElement >& elements);

}
