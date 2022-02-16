#ifndef HTTP_PROXY_FLOW_H
#define HTTP_PROXY_FLOW_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>
#include <utility>

using namespace boost;

/**
 * Класс, управляющий потоком от клиента прокси.
 * Объект класса сам принимает решение о времени своей жизни,
 * если все указатели на него уничтожены. Такая механика нужно чтобы не сохранять
 * его в контейнер в Proxy и затем не удалять, его обкладываясь мьютексами.
 */
class Flow : public std::enable_shared_from_this<Flow>, boost::noncopyable {
public:
    static std::shared_ptr<Flow> create(std::shared_ptr<asio::io_service> ios);
    void handle();
    asio::ip::tcp::socket &inSocket();

private:
    explicit Flow(std::shared_ptr<asio::io_service> &&ios);

    void destroy();
    void onRequestFirstLineReceived(const boost::system::error_code &err, std::size_t bytes);
    void onRequestHeadersReceived(const boost::system::error_code &err, std::size_t bytes);
    void onRequestBodyReceived(const boost::system::error_code &err, std::size_t bytes);
    void onHostResolve(const boost::system::error_code& err, const asio::ip::tcp::resolver::iterator& iterator);
    void onConnectionEstablish(const boost::system::error_code& err, const asio::ip::tcp::resolver::iterator& iterator);
    void onRequestSend(const boost::system::error_code& err, std::size_t bytes);
    void onResponseRead(const boost::system::error_code &err, std::size_t bytes);
    void onResponseSend(const boost::system::error_code &err, std::size_t bytes);

    std::shared_ptr<asio::io_service> _ios;
    std::unique_ptr<asio::ip::tcp::resolver> _resolver;
    std::unique_ptr<asio::ip::tcp::socket> _outSocket;
    std::unique_ptr<asio::ip::tcp::socket> _inSocket;
    std::shared_ptr<Flow> _self;
    asio::streambuf _request;
    asio::streambuf _response;
    std::string _patchedHeaders;
    std::string  _host;
    unsigned _port;


};

#endif //HTTP_PROXY_FLOW_H
