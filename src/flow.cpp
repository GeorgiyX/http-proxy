#include "flow.h"
#include "http.h"
#include <iostream>
#include <utility>

Flow::Flow(std::shared_ptr<asio::io_service> &&ios) :
        _ios(std::move(ios)),
        _resolver(std::make_unique<asio::ip::tcp::resolver>(*_ios)),
        _outSocket(std::make_unique<asio::ip::tcp::socket>(*_ios)),
        _inSocket(std::make_unique<asio::ip::tcp::socket>(*_ios)),
        _self(),
        _request(),
        _response(),
        _patchedHeaders() {
}

asio::ip::tcp::socket &Flow::inSocket() {
    return *_inSocket;
}

std::shared_ptr<Flow> Flow::create(std::shared_ptr<asio::io_service> ios) {
    auto flow = std::shared_ptr<Flow>(new Flow(std::move(ios)));
    flow->_self = flow->shared_from_this();
    return flow;
}

void Flow::destroy() {
    _self.reset();
}

void Flow::handle() {
    asio::async_read_until(inSocket(), _request, "\r\n\r\n",
                     std::bind(&Flow::onRequestBodyReceived, this,
                               std::placeholders::_1, std::placeholders::_2));
}

void Flow::onRequestBodyReceived(const system::error_code &err, std::size_t bytes) {
    if (err.value()) {
        std::cerr << "Error in async_read (onRequestBodyReceived): " << err.value() << " " << err.message() << std::endl;
        destroy();
        return;
    }

    /* Извлечение стартовой строки запроса: */
    std::string requestLine;
    std::istream requestStream(&_request);
    std::getline(requestStream, requestLine, '\r');
    requestStream.get();
    std::cout << requestLine << std::endl;

    /* Парсинг стартовой строки: */
    std::istringstream requestLineStream(requestLine);

    std::string requestMethod;
    requestLineStream >> requestMethod;

    std::string requestTarget;
    requestLineStream >> requestTarget;

    std::string httpVersion;
    requestLineStream >> httpVersion;

    if (httpVersion != HTTP1_1) {
        std::cerr << "Не поддерживаемая версия HTTP: " << httpVersion << std::endl;
        destroy();
        return;
    }

    auto hostPortPath = HostPortPath::fromRequestTarget(requestTarget);
    _host = hostPortPath.host;
    _port = hostPortPath.port;
    _patchedHeaders += requestMethod + " " + hostPortPath.path + " " + httpVersion + "\r\n";

    std::string header;
    while (true) {
        std::getline(requestStream, header, '\r');
        requestStream.get();
        if (header.empty()) {
            break;
        }

        if (header.find(PROXY_CONNECTION) == std::string::npos) {
            _patchedHeaders += header + "\r\n";
        }
    }
    _patchedHeaders += "\r\n";
    std::cout << _patchedHeaders;
    requestStream.seekg(2, std::ios_base::cur);
    _request.consume(requestStream.tellg());
    std::cout << _request.size();

    asio::ip::tcp::resolver::query resolverQuery(_host,
                                                 std::to_string(_port),
                                                 asio::ip::tcp::resolver::query::numeric_service);
    _resolver->async_resolve(resolverQuery, std::bind(&Flow::onHostResolve, this,
                                                      std::placeholders::_1, std::placeholders::_2));
}

void Flow::onHostResolve(const system::error_code &err, const asio::ip::tcp::resolver::iterator &iterator) {
    if (err.value()) {
        std::cerr << "Error in async_resolve (onHostResolve): " << err.value() << " " << err.message() << std::endl;
        destroy();
        return;
    }
    asio::async_connect(*_outSocket,
                        iterator,
                        std::bind(&Flow::onConnectionEstablish, this,
                                  std::placeholders::_1, std::placeholders::_2));


}

void Flow::onConnectionEstablish(const system::error_code &err, const asio::ip::tcp::resolver::iterator &iterator) {
    if (err.value()) {
        std::cerr << "Error in async_connect (onConnectionEstablish): " << err.value() << " " << err.message() << std::endl;
        destroy();
        return;
    }

    asio::async_write(*_outSocket,
                      asio::buffer(_patchedHeaders),
                      [this](const boost::system::error_code &ec,
                             std::size_t bytes_transferred) {
                          asio::async_write(*_outSocket, asio::buffer(_request.data()), //todo
                                            std::bind(&Flow::onRequestSend, this,
                                                      std::placeholders::_1, std::placeholders::_2));
                      });
}

void Flow::onRequestSend(const system::error_code &err, std::size_t bytes) {
    if (err.value()) {
        std::cerr << "Error in async_write (onRequestSend): " << err.value() << " " << err.message() << std::endl;
        destroy();
        return;
    }

    _outSocket->shutdown(asio::ip::tcp::socket::shutdown_send);
    asio::async_read(*_outSocket, _response,
                     std::bind(&Flow::onResponseRead, this,
                               std::placeholders::_1, std::placeholders::_2));
}

void Flow::onResponseRead(const system::error_code &err, std::size_t bytes) {
    if (err.value() != asio::error::eof) {
        std::cerr << "Error in async_read (onResponseRead): " << err.value() << " " << err.message() << std::endl;
        destroy();
        return;
    }

    std::cout << static_cast<const char *>(_response.data().data());

    asio::async_write(*_inSocket,
                      asio::buffer(_response.data()),
                      std::bind(&Flow::onResponseSend, this,
                                std::placeholders::_1, std::placeholders::_2));
}

void Flow::onResponseSend(const system::error_code &err, std::size_t bytes) {
    if (err.value()) {
        std::cerr << "Error in async_write (onResponseSend): " << err.value() << " " << err.message() << std::endl;
        destroy();
        return;
    }
    _inSocket->shutdown(asio::ip::tcp::socket::shutdown_send);
    _inSocket->close();
    std::cout << "Proxy done: " << _host << ":" << _port << std::endl;
}
