#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace StarkwareCppWrapper
{

class ApiException: public std::exception
{
  public:
    explicit ApiException( int32_t theCode )
        : code( theCode )
    {
    }

    std::string explain() const noexcept
    {
        std::ostringstream errorString;
        errorString << "Api error with code: ";
        errorString << code;

        return errorString.str();
    }

  private:
    int32_t code;
};

#define apiCheckResult( errorCode )                                                                                    \
    if( errorCode )                                                                                                                        \
    {                                                                                                                                      \
        throw ApiException( errorCode );                                                 \
    }

} // namepsace StarkwareCppWrapper

