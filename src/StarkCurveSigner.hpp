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

    // TODO: implement generate_k
    starkware::Signature signMessage( const EncodableIface& message, const starkware::PrimeFieldElement::ValueType& k ) const;

  private:
    KeyPair keyPair;
};

}
