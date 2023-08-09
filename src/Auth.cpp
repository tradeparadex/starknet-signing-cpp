#include <chrono>
#include <cstring>

#include "Auth.hpp"
#include "UtilsImpl.hpp"

namespace signer
{

Auth::Auth()
{
    using namespace std::chrono;

    now = duration_cast< seconds >( system_clock::now().time_since_epoch() );
    expiry = now + 24 * 60 * 60s;
}

void Auth::setNow( std::chrono::seconds value )
{
    now = value;
}

void Auth::setExpiry( std::chrono::seconds value )
{
    expiry = value;
}

std::chrono::seconds Auth::getNow() const
{
    return now;
}

std::chrono::seconds Auth::getExpiry() const
{
    return expiry;
}

std::vector< starkware::PrimeFieldElement > Auth::pedersenEncode() const
{
    using namespace starkware;

    const char* strRequest = "Request(method:felt,path:felt,body:felt,timestamp:felt,expiration:felt)";
    const char* strMethod = "POST";
    const char* strPath = "/v1/auth";
    const char* strBody = "";

    const PrimeFieldElement request = PrimeFieldElement::FromBigInt( getSelectorFromName( strRequest, std::strlen( strRequest ) ) );
    const PrimeFieldElement method = strToFelt( strMethod, std::strlen( strMethod ) );
    const PrimeFieldElement path = strToFelt( strPath, std::strlen( strPath ) );
    const PrimeFieldElement body = strToFelt( strBody, std::strlen( strBody ) );
    const PrimeFieldElement timestamp = PrimeFieldElement::FromUint( now.count() );
    const PrimeFieldElement expiration = PrimeFieldElement::FromUint( expiry.count() );

    return { request, method, path, body, timestamp, expiration };
}

} // namespace signer