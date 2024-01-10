#include "Account.hpp"
#include "StarknetDomain.hpp"
#include "Order.hpp"
#include "Message.hpp"
#include "StarkCurveSigner.hpp"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

// Callback function to write the response to a string
size_t writeCallback(char* ptr, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append(ptr, totalSize);
    return totalSize;
}

int postOrder(const signer::Account& account, const std::string& jwtToken)
{
    using namespace starkware;
    using namespace signer;

    const auto& address = account.getAddress();
    const StarkCurveSigner& signer = account.getSigner();

    StarknetDomain starknetDomain( account.getChainId() );
    const Order order( "ETH-USD-PERP", OrderSide::Buy, OrderType::Market, 0.1, std::nullopt, "123454321" );
    const Message message( address, std::make_shared< StarknetDomain >( starknetDomain ), std::make_shared< Order >( order ) );

    const auto h = message.hash();
    const auto sig = signer.signMessage( message );
    const std::string payload = order.compileOrderRequestPayload( sig );
    try
    {
        // Create a cURLpp request
        curlpp::Easy request;

        // Set the request URL
        std::string paradex_http_url = "https://api.testnet.paradex.trade/v1";
        std::string path = "/orders";
        request.setOpt( new curlpp::options::Url( paradex_http_url + path ) );

        // Set request headers
        std::list< std::string > headers;
        headers.push_back( "Content-Type: application/json" );
        headers.emplace_back( "Authorization: Bearer " + jwtToken );
        request.setOpt( new curlpp::options::HttpHeader( headers ) );

        // Set request payload
        request.setOpt( new curlpp::options::PostFields( payload ) );
        request.setOpt( new curlpp::options::PostFieldSize( payload.length() ) );

        // Set the custom write function to capture the response
        std::string response;
        request.setOpt(new curlpp::options::WriteFunction(std::bind(&writeCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &response)));

        // Perform the request
        request.perform();

        // Print the captured response
        std::cout << "Response: " << response << std::endl;

        // Cleanup cURLpp
        curlpp::terminate();
    }
    catch( curlpp::RuntimeError& e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch( curlpp::LogicError& e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

int main()
{
    using namespace starkware;
    using namespace signer;

    // Initialize cURLpp
    curlpp::initialize();

    // Stark key and address
    const auto address = 0xabc_Z;
    constexpr BigInt< 4 > privateKey = 0xabc_Z;
    const std::string chainId = "PRIVATE_SN_POTC_SEPOLIA";

    KeyPair keyPair( privateKey );
    Account account( PrimeFieldElement::FromBigInt( address ), chainId, keyPair );
    const std::string jwtToken = account.getJwtToken( "https://api.testnet.paradex.trade/v1/auth" );
    return postOrder( account, jwtToken );
}
