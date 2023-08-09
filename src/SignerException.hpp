#pragma once

#include <exception>
#include <string>


namespace signer
{

class SignerException: public std::exception
{
  public:
    explicit SignerException( const std::string& theMessage )
        : message( theMessage )
    {
    }

    std::string explain() const noexcept
    {
        return message;
    }

  private:
    std::string message;
};

#define ASSERT( condition, msg )                                                                                                              \
    if( !( condition ) )                                                                                                                   \
    {                                                                                                                                      \
        throw SignerException( msg );                                                                                 \
    }

} // namespace signer

