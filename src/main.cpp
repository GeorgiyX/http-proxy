#include <iostream>
#include "proxy.h"

int main() {
    std::cout << "Start proxy" << std::endl;
    Proxy proxy(8);
    try {
        proxy.start(80);
    } catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        proxy.stop();
    }
}