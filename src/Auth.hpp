#pragma once

#include <chrono>
#include <starkware/algebra/prime_field_element.h>

#include "HashableIface.hpp"

namespace signer
{

class Auth: public HashableIface
{
  public:
    explicit Auth();

    void setNow(std::chrono::seconds value);
    void setExpiry(std::chrono::seconds value);

    std::chrono::seconds getNow() const;
    std::chrono::seconds getExpiry() const;

    virtual std::vector< starkware::PrimeFieldElement> pedersenEncode() const override;

  private:
    std::chrono::seconds now;
    std::chrono::seconds expiry;

};

} // namespace signer