#include "proxy.h"
#include "flow.h"

const unsigned BACKLOG_SIZE = 42;

Proxy::Proxy(size_t threadsCount) :
        _ios(),
        _work(asio::make_work_guard(*_ios)),
        _threadPool(),
        _acceptor(),
        _isStopped(false) {
    _threadPool.reserve(threadsCount);
}

void Proxy::start(unsigned port) {
    for (unsigned i = 0; i < _threadPool.capacity(); i++) {
        _threadPool.push_back(std::make_unique<std::thread>([this](){
            _ios->run();
        }));
    }

    auto endpoint = asio::ip::tcp::endpoint(asio::ip::address_v4::any(), port);
    _acceptor = std::make_unique<asio::ip::tcp::acceptor>(_ios, std::move(endpoint));
    _acceptor->listen(BACKLOG_SIZE);
    accept();
}

void Proxy::stop() {
    /* Инициируем завершение метода run() io_service и дожидаемся треды */
    _isStopped.store(true);
    _work.reset();
    for (auto &thread : _threadPool) {
        thread->join();
    }
}

void Proxy::knockout() {
    _isStopped.store(true);
    _ios->stop();
    for (auto &thread : _threadPool) {
        thread->join();
    }
}

void Proxy::accept() {
    auto flow = Flow::create(_ios);
    _acceptor->async_accept(flow->inSocket(), [this, flow] (const boost::system::error_code &err) {
        if (!err) {
            std::cerr << "Error in async_accept: " << err.value() << " " << err.message() << std::endl;
        } else {
            flow->handle();
        }

        if (!_isStopped.load()) {
            _acceptor->close();
            return;
        }

        accept();
    });
}
