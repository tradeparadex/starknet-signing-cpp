#include <iostream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <nlohmann/json.hpp>

#include "Account.hpp"
#include "UtilsImpl.hpp"
#include "SignerException.hpp"

namespace signer
{

Account::Account( const starkware::PrimeFieldElement& theAddress, const std::string& theChainId, const KeyPair& theKeyPair )
    : address( theAddress )
    , signer( StarkCurveSigner( theKeyPair ) )
{
    chainId = strToFelt( theChainId.c_str(), theChainId.size() ).ToStandardForm();
}

Account::Account( const starkware::PrimeFieldElement& theAddress, const Uint256& theChainId, const KeyPair& theKeyPair )
    : address( theAddress )
    , chainId( theChainId )
    , signer( StarkCurveSigner( theKeyPair ) )
{
}

const Uint256& Account::getChainId() const
{
    return chainId;
}

const starkware::PrimeFieldElement& Account::getAddress() const
{
    return address;
}

const StarkCurveSigner& Account::getSigner() const
{
    return signer;
}

StarknetDomain Account::createStarknetDomain() const
{
    return StarknetDomain( chainId );
}

std::string Account::extractJwtToken(const std::string& jsonString)
{
    try
    {
        auto json = nlohmann::json::parse(jsonString);
        return json["jwt_token"];
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return ""; // Return an empty string on error
    }
}

std::string Account::getJwtToken( const std::string& url ) const
{
    using namespace std::chrono;
    using namespace starkware;

    const Auth auth;
    const StarknetDomain starknetDomain = createStarknetDomain();

    const Message message( address, std::make_shared< StarknetDomain >( starknetDomain ), std::make_shared< Auth >( auth ) );
    const auto hash = message.hash();
    const auto signature = signer.signMessage( message );

    const uint64_t now = auth.getNow().count();
    const uint64_t expiry = auth.getExpiry().count();

    try
    {
        const std::string data = "{}";
        std::string accountHeader = "PARADEX-STARKNET-ACCOUNT: %1";
        {
            std::ostringstream s;
            s << address;

            std::string strAddress = s.str();
            removeLeadingZeroes( &strAddress );

            replace( &accountHeader, "%1", strAddress );
        }

        std::string signatureHeader = "PARADEX-STARKNET-SIGNATURE: [\"%1\", \"%2\"]";
        replace( &signatureHeader, "%1", signature.first );
        replace( &signatureHeader, "%2", signature.second );

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

        return extractJwtToken(responseStream.str());
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
    catch( SignerException& e )
    {
        std::cerr << e.what() << std::endl;
        throw e;
    }
}

} // namespace signer
