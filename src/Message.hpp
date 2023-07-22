#pragma once

#include <vector>
#include "HashableIface.hpp"

namespace signer
{

class Message: public HashableIface
{
  public:
    Message(PrimeFieldElement theAccountAddress, HashableIface::Pointer theDomain, HashableIface::Pointer thePrimaryType );

    virtual std::vector< starkware::PrimeFieldElement > pedersenEncode() const override;

  private:

    PrimeFieldElement accountAddress;
    HashableIface::Pointer domain;
    HashableIface::Pointer primaryType;
};

} // namespace signer
