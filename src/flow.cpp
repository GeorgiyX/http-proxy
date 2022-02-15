#include "flow.h"

Flow::Flow(std::shared_ptr<asio::io_service> &&ios) :
        _ios(std::move(ios)),
        _inSocket(std::make_unique<asio::ip::tcp::socket>(*ios)),
        _self() {
}

asio::ip::tcp::socket &Flow::inSocket() {
    return *_inSocket;
}

std::shared_ptr<Flow> Flow::create(std::shared_ptr<asio::io_service> ios) {
    auto flow = std::shared_ptr<Flow>(new Flow(std::move(ios)));
    flow->_self = flow->shared_from_this();
    return flow;
}

void Flow::destroy()  {
    _self.reset();
}
