#pragma once

#include <chrono>

#include "StarknetDomain.hpp"
#include "Order.hpp"
#include "Auth.hpp"
#include "Message.hpp"

namespace signer
{

StarknetDomain getStarknetDomain();

Order getOrder( std::chrono::milliseconds timestamp );

Auth getAuth( std::chrono::seconds now, std::chrono::seconds expiry );

Message getOrderMessage();

} // namespace signer