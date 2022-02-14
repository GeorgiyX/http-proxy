#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

int main() {
    std::cout << "Boost asio, hello!" << std::endl;
    asio::io_service ios;
    asio::ip::tcp protocol = asio::ip::tcp::v4();
    asio::ip::tcp::acceptor acceptor(ios);
    boost::system::error_code err;
    acceptor.open(protocol, err);

    if (err.value()) {
        std::cout << "Не удалось открыть acceptor сокет: " << err.message() << std::endl;
        exit(err.value());
    }

    return 0;




    return 0;
}
