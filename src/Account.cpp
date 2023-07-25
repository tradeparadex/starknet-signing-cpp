#include <stdexcept>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "starkware/crypto//elliptic_curve_constants.h"

#include "Account.hpp"

namespace signer
{

Account::Account( const starkware::PrimeFieldElement& theAddress, const Uint256& theChainId, const KeyPair& theKeyPair )
    : address( theAddress )
    , chainId( theChainId )
    , signer( StarkCurveSigner( theKeyPair ) )
{
}

StarknetDomain Account::createStarknetDomain() const
{
    return StarknetDomain( chainId );
}

// static
template< class T >
void Account::replace( std::string* header, const char* token, T val )
{
    constexpr uint8_t numTokenLen = 2;

    std::ostringstream s;
    s << val;

    // Find the position of "%S" in the original string
    size_t pos = header->find( token );

    // Check if "%S" was found in the string
    if( pos == std::string::npos )
    {
        throw new std::length_error( "Couldn't find %1" );
    }

    header->replace( pos, numTokenLen, s.str() );
}

void Account::removeLeadingZeroes(std::string* value) {
    std::string& hexValue = *value;

    // Check if the first 2 characters are "0x"
    if (hexValue.length() < 2 || hexValue.substr(0, 2) != "0x") {
        return;
    }

    size_t startPos = 2;

    // Find the position of the first non-zero character after "0x"
    while (startPos < hexValue.length() && hexValue[startPos] == '0') {
        startPos++;
    }

    // Extract the substring starting from the first non-zero character
    if (startPos < hexValue.length()) {
        hexValue = "0x" + hexValue.substr(startPos);
    } else {
        // If the entire string is zeros, keep at least one zero.
        hexValue = "0x0";
    }
}

std::string Account::getJwtToken() const
{
    using namespace std::chrono;

    const Auth auth;
    const StarknetDomain starknetDomain = createStarknetDomain();

    const Message message( address, std::make_shared< StarknetDomain >( starknetDomain ), std::make_shared< Auth >( auth ) );

    const auto hash = message.hash();

    // TODO: implement generate_k
    const auto k = 0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z;
    const auto signature = signer.signMessage( message );
    const uint64_t now = auth.getNow().count();
    const uint64_t expiry = auth.getExpiry().count();

    try
    {
        std::string url = "https://api.testnet.paradex.trade/v1/auth";
        std::string data = "{}"; // Replace this with your POST request data (if any)

        std::string accountHeader = "PARADEX-STARKNET-ACCOUNT: %1";
        {
            std::ostringstream s;
            s << address;

            std::string strAddress = s.str();
            removeLeadingZeroes(&strAddress);

            replace( &accountHeader, "%1", strAddress);
        }

        std::string signatureHeader = "PARADEX-STARKNET-SIGNATURE: [\"%1\", \"%2\"]";
        replace( &signatureHeader, "%1", signature.first );
        replace( &signatureHeader, "%2", signature.second.ToStandardForm().InvModPrime( starkware::GetEcConstants().k_order ) );

        std::string hashHeader = "PARADEX-STARKNET-MESSAGE-HASH: %1";
        replace( &hashHeader, "%1", hash );

        std::string nowHeader = "PARADEX-TIMESTAMP: %1";
        std::string strNow = std::to_string( now );
        replace( &nowHeader, "%1", strNow );

        std::string expiryHeader = "PARADEX-SIGNATURE-EXPIRATION: %1";
        std::string strExpiry = std::to_string( expiry );
        replace( &expiryHeader, "%1", strExpiry );

        // Set the required headers
        std::list< std::string > headers{ accountHeader, signatureHeader, hashHeader, nowHeader, expiryHeader };

        // Initialize cURLpp
        curlpp::Cleanup cleanup;
        curlpp::Easy request;

        // Set the URL and request data
        request.setOpt( new curlpp::options::Url( url ) );
        request.setOpt( new curlpp::options::PostFields( data ) );
        request.setOpt( new curlpp::options::PostFieldSize( data.length() ) );

        // Set the headers
        request.setOpt( new curlpp::options::HttpHeader( headers ) );

        // Perform the POST request and get the response
        std::ostringstream responseStream;
        request.setOpt( new curlpp::options::WriteStream( &responseStream ) );
        request.perform();

        return responseStream.str();
    }
    catch( curlpp::RuntimeError& e )
    {
        std::cerr << "curlpp::RuntimeError: " << e.what() << std::endl;
        throw e;
    }
    catch( curlpp::LogicError& e )
    {
        std::cerr << "curlpp::LogicError: " << e.what() << std::endl;
        throw e;
    }
}

} // namespace signer
