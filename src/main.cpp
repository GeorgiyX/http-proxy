#include <iostream>
#include "proxy.h"

int main() {
    std::cout << "Hello World!\n";
    Proxy proxy(8);
    try {
        proxy.start(5000);
    } catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        proxy.stop();
    }
}