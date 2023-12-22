#pragma once

#include <utility>

#include <starkware/algebra/prime_field_element.h>
#include <starkware/algebra/elliptic_curve.h>
#include <starkware/crypto/ecdsa.h>

#include "EncodableIface.hpp"

namespace signer
{

struct KeyPair
{
    explicit KeyPair( const starkware::PrimeFieldElement::ValueType& thePrivateKey,
        const std::optional< starkware::EcPoint< starkware::PrimeFieldElement > >& thePublicKey = std::nullopt );
    KeyPair( const KeyPair& ) = default;

    starkware::PrimeFieldElement::ValueType privateKey;
    starkware::EcPoint< starkware::PrimeFieldElement > publicKey;
};

class StarkCurveSigner
{
  public:
    StarkCurveSigner( const KeyPair& theKeyPair );

    // TODO: implement c++ generate_k
    starkware::Signature signMessage( const EncodableIface& message, const starkware::PrimeFieldElement::ValueType& k ) const;
    starkware::Signature signMessage( const EncodableIface& message ) const;

    bool verifyEcdsa( const starkware::PrimeFieldElement& hash, const starkware::Signature& signature ) const;

  private:
    KeyPair keyPair;
};

} // signer
