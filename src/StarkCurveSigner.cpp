#include "Ecdsa.hpp"
#include "StarkCurveSigner.hpp"
#include "UtilsImpl.hpp"

#if ENABLE_CPP
#include <starkware/crypto/elliptic_curve_constants.h>
#endif

namespace signer
{

KeyPair::KeyPair( const PrimeFieldElement::ValueType& thePrivateKey,
    const std::optional< starkware::EcPoint< starkware::PrimeFieldElement > >& thePublicKey )
    : privateKey( thePrivateKey )
    , publicKey( PrimeFieldElement::Zero(), PrimeFieldElement::Zero() )
{
    if( thePublicKey )
    {
        publicKey = *thePublicKey;
    }
    else
    {
        publicKey = GetPublicKey( thePrivateKey );
    }
}

StarkCurveSigner::StarkCurveSigner( const KeyPair& theKeyPair )
    : keyPair( theKeyPair )
{
}

starkware::Signature StarkCurveSigner::signMessage( const EncodableIface& message, const starkware::PrimeFieldElement::ValueType& k ) const
{
    using namespace starkware;

    const auto encodedMessage = message.pedersenEncode();
    const PrimeFieldElement messageHash = hashElements( encodedMessage );

#if ENABLE_CPP
    const Signature signature =  SignEcdsa( keyPair.privateKey, messageHash, k );
    const PrimeFieldElement sInv = PrimeFieldElement::FromBigInt(signature.second.ToStandardForm().InvModPrime( GetEcConstants().k_order ));
    return {signature.first, sInv};
#else
    const PrimeFieldElement privateKeyFelt = PrimeFieldElement::FromBigInt( keyPair.privateKey );
    const PrimeFieldElement kFelt = PrimeFieldElement::FromBigInt( k );
    return StarkwareCppWrapper::Ecdsa::ecdsaSign( privateKeyFelt, messageHash, kFelt );
#endif
}

starkware::Signature StarkCurveSigner::signMessage( const EncodableIface& message ) const
{
    // TODO: use ENABLE_CPP when cpp version implemented
    const auto encodedMessage = message.pedersenEncode();
    const PrimeFieldElement messageHash = hashElements( encodedMessage );

    const PrimeFieldElement privateKeyFelt = PrimeFieldElement::FromBigInt( keyPair.privateKey );
    return StarkwareCppWrapper::Ecdsa::ecdsaSign( privateKeyFelt, messageHash );
}

bool StarkCurveSigner::verifyEcdsa(const starkware::PrimeFieldElement& hash, const starkware::Signature& signature) const
{
#if ENABLE_CPP
    return VerifyEcdsa( keyPair.publicKey, hash, signature );
#else
    return StarkwareCppWrapper::Ecdsa::ecdsaVerify( keyPair.publicKey.x, hash, signature );
#endif
}

} // namepsace signer
