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

    std::shared_ptr<asio::io_service> _ios;
    std::unique_ptr<asio::ip::tcp::socket> _inSocket;
    std::shared_ptr<Flow> _self;
};

#endif //HTTP_PROXY_FLOW_H
