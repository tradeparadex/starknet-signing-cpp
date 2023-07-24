#pragma once

#include <utility>
#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto/ecdsa.h>

#include "EncodableIface.hpp"

namespace signer
{

struct KeyPair
{
    KeyPair( const starkware::PrimeFieldElement::ValueType & thePrivateKey, const starkware::PrimeFieldElement& thePublicKey );
    KeyPair( const KeyPair& ) = default;

    KeyPair getPublicKey() const;

    starkware::PrimeFieldElement::ValueType privateKey;
    starkware::PrimeFieldElement publicKey;
};

class StarkCurveSigner
{
  public:
    StarkCurveSigner( const KeyPair& theKeyPair );

    starkware::Signature signMessage( const EncodableIface& message, const starkware::PrimeFieldElement::ValueType& k = numK ) const;

  private:
    static constexpr starkware::BigInt< 4 > numK = starkware::operator""_Z<'0', 'x', '7', '0', 'e', '9', '2', 'b', '6', '7', '2', '2', 'b',
                                            '8', '5', 'a', '0', 'd', 'c', '1', '3', '8', '2', '8', '1', '7', '0', '2', 'e', 'a', 'b', '9',
                                            'f', 'b', '2', '8', 'e', '0', '8', 'c', '8', 'd', '8', '6', '4', 'd', '1', '1', '9', 'c', 'a',
                                            'd', '9', '9', 'f', 'a', '6', '1', '5', '6', '2', 'b', '9', '2', '0' > ();
    KeyPair keyPair;
};

}
