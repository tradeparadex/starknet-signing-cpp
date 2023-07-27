#include "StarkCurveSigner.hpp"
#include "UtilsImpl.hpp"

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
    const auto encodedMessage = message.pedersenEncode();
    const PrimeFieldElement messageHash = hashElements( encodedMessage );

    return SignEcdsa( keyPair.privateKey, messageHash, k );
}

bool StarkCurveSigner::verifyEcdsa(const starkware::PrimeFieldElement& hash, const starkware::Signature& signature) const
{
    return VerifyEcdsa( keyPair.publicKey, hash, signature );
}

} // namepsace signer
