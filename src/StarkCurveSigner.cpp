#include "StarkCurveSigner.hpp"
#include "UtilsImpl.hpp"

namespace signer
{

KeyPair::KeyPair( const starkware::PrimeFieldElement::ValueType& thePrivateKey, const starkware::PrimeFieldElement& thePublicKey )
    : privateKey( thePrivateKey )
    , publicKey( thePublicKey )
{
}

KeyPair KeyPair::getPublicKey() const
{
    const auto ecPoint = starkware::GetPublicKey( privateKey );
    return { privateKey, ecPoint.x };
}

StarkCurveSigner::StarkCurveSigner( const KeyPair& theKeyPair )
    : keyPair( theKeyPair )
{
}

starkware::Signature StarkCurveSigner::signMessage( const EncodableIface& message, const starkware::PrimeFieldElement::ValueType& k /* = numK */) const
{
    const auto encodedMessage = message.pedersenEncode();
    const PrimeFieldElement messageHash = hashElements( encodedMessage );

    return SignEcdsa( keyPair.privateKey, messageHash, k );
}

}
