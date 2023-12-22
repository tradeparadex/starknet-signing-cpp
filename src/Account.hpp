#pragma once

#include <starkware/algebra/prime_field_element.h>

#include "StarknetDomain.hpp"
#include "Auth.hpp"
#include "Message.hpp"
#include "StarkCurveSigner.hpp"

namespace signer
{

class Account
{
  public:
    Account( const starkware::PrimeFieldElement& theAddress, const std::string& theChainId, const KeyPair& theKeyPair );
    Account( const starkware::PrimeFieldElement& theAddress, const Uint256& theChainId, const KeyPair& theKeyPair );

    const Uint256& getChainId() const;
    const starkware::PrimeFieldElement& getAddress() const;
    const StarkCurveSigner& getSigner() const;

    StarknetDomain createStarknetDomain() const;
    Message createAuthMessage() const;

    std::string getJwtToken( const std::string& url ) const;

  private:
    static std::string extractJwtToken(const std::string& jsonString);

    Uint256 chainId;
    StarkCurveSigner signer;
    starkware::PrimeFieldElement address;
};

} // namespace signer
