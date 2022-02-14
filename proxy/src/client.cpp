#include "client.h"
#include "utils.h"
#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

void client_main() {
    boost::system::error_code err;
    auto address = asio::ip::address::from_string("localhost", err);
    handleErr(err);
    asio::io_service ios;
    asio::ip::tcp::
}

