#include "utils.h"
#include <iostream>

void handleErr(const boost::system::error_code &err, const std::string &message) {
    if (err.value()) {
        std::cerr << message << "; code =\"" << err.value() << "\", message =\""
        << err.message() << "\"" << std::endl;
        exit(err.value());
    }
}

