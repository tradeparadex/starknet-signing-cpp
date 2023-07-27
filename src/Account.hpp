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

    StarknetDomain createStarknetDomain() const;
    Message createAuthMessage() const;

    std::string getJwtToken(const std::string& url) const;

  private:
    template <class T>
    static void replace(std::string *header, const char* token, T val);

    static void removeLeadingZeroes(std::string* value);

    Uint256 chainId;
    StarkCurveSigner signer;
    starkware::PrimeFieldElement address;
};

} // namespace signer
