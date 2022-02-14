#include "utils.h"
#include <iostream>

void handleErr(const boost::system::error_code &err) {
    if (err.value()) {
        std::cerr << "Error. Code = \"" << err.value() << "\". Message = \""
        << err.message() << "\"" << std::endl;
        exit(err.value());
    }
}

