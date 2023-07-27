#include <chrono>
#include <vector>
#include <iostream>
#include <string>

#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto/ecdsa.h>

#include "Utils.hpp"
#include "UtilsImpl.hpp"
#include "StarknetDomain.hpp"
#include "Auth.hpp"
#include "Order.hpp"
#include "Message.hpp"
#include "StarkCurveSigner.hpp"

using namespace starkware;
using namespace signer;
using namespace std::chrono;
StarknetDomain getStarknetDomain()
{
    using namespace starkware;

    const BigInt< 4 > chainId = 0x505249564154455F534E5F504F54435F474F45524C49_Z;
    StarknetDomain domain( chainId );

    return domain;
}

Order getOrder(milliseconds timestamp)
{
    const char* strMarket = "ETH-USD-PERP";
    constexpr OrderType orderType = OrderType::Market;
    constexpr OrderSide orderSide = OrderSide::Buy;
    constexpr double size = 0.1;

    Order order(strMarket, orderSide, orderType, size);
    order.setTimestamp(timestamp);

    return order;
}

Message getOrderMessage()
{
    constexpr milliseconds timestamp = 1690187319619ms;
    const auto address = 0x1F06D2232E3EB0D8BD4B9294A930D553F610A0D7B24BC52D9472C7BDA478927_Z;

    const PrimeFieldElement accountAddress = PrimeFieldElement::FromBigInt( address );
    Order order = getOrder(timestamp);
    StarknetDomain starknetDomain = getStarknetDomain();

    Message message( accountAddress, std::make_shared< StarknetDomain >( starknetDomain ), std::make_shared< Order >( order ) );
    return message;
}

int main() {

// Get the current time before the operation
// THis is test comment
    auto prepare = high_resolution_clock::now(); 
    Prng prng;
    using ValueType = PrimeFieldElement::ValueType;
    const auto privateKey = ValueType::RandomBigInt( &prng );
    KeyPair keyPair( privateKey );
    
    StarkCurveSigner signer( keyPair );
    auto start = high_resolution_clock::now(); 
    std::cout << "prepare:" << duration_cast<microseconds>(start - prepare).count() << std::endl;
    for(int i = 0; i < 10; i++) {
        auto start_iter = high_resolution_clock::now(); 
        Message message = getOrderMessage();
        auto order_msg = high_resolution_clock::now(); 
        const auto h = message.hash();
        auto msg_hash = high_resolution_clock::now(); 
        const auto k = 0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z;

        const Signature res = signer.signMessage( message, k );
        auto signature = high_resolution_clock::now(); 
        // if (i % 100 == 0){
            std::cout << i << ":" << duration_cast<microseconds>(high_resolution_clock::now() - start_iter).count() << std::endl;
            std::cout << "order_msg:" << duration_cast<microseconds>(order_msg - start_iter).count() << std::endl;
            std::cout << "msg_hash:" << duration_cast<microseconds>(msg_hash - start_iter).count() << std::endl;
            std::cout << "signature:" << duration_cast<microseconds>(signature - start_iter).count() << std::endl;
        // }
    }
    auto end = high_resolution_clock::now(); // Get the current time after the operation
    auto duration = duration_cast<microseconds>(end - start).count(); // Calculate the time taken in microseconds
    std::cout << "Latency of signing: " << duration << " microseconds" << std::endl;

    return 0;
}

