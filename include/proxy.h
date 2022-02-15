#ifndef HTTP_PROXY_PROXY_H
#define HTTP_PROXY_PROXY_H

#include "flow.h"

#include <boost/asio.hpp>

#include <thread>
#include <atomic>
#include <memory>
#include <utility>
#include <iostream>
#include <set>

using namespace boost;

class Proxy {
public:
    explicit Proxy(size_t threadsCount);
    void start(unsigned port);

    /** Мягкая остановка (текущие операции завершаются) */
    void stop();

    /** Быстрая остановка (прерывает все операции) */
    void knockout();
private:
    void accept();

    std::shared_ptr<asio::io_service> _ios;
    asio::executor_work_guard<asio::io_service::executor_type> _work;
    std::vector<std::unique_ptr<std::thread>> _threadPool;
    std::unique_ptr<asio::ip::tcp::acceptor> _acceptor;
    std::atomic<bool> _isStopped;

    /*struct {
        std::set<std::shared_ptr<InFlow>> flows;
        std::mutex mutex;
    } _aloneFlows;

    struct {
        std::map<std::shared_ptr<OutFlow>, std::shared_ptr<InFlow>> flows;
        std::mutex mutex;
    } _pairedFlows;*/
};


#endif //HTTP_PROXY_PROXY_H
