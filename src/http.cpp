#include <stdexcept>
#include "http.h"

const std::string HTTPS = "https://";
const std::string HTTP = "http://";
const std::string HTTP1_1 = "HTTP/1.1";
const std::string PROXY_CONNECTION = "Proxy-Connection";

HostPortPath HostPortPath::fromRequestTarget(const std::string &requestTarget) {
    HostPortPath result;
    size_t pos = 0;
    if (requestTarget.find(HTTPS, 0) != std::string::npos) {
        result.port = 443;
        pos += HTTPS.length();
    } else if (requestTarget.find(HTTP, 0) != std::string::npos) {
        result.port = 80;
        pos += HTTP.length();
    } else {
        throw std::runtime_error("Не верный формат стартовой строки запроса");
    }

    auto slashPos = requestTarget.find('/', pos);
    result.host = requestTarget.substr(pos, slashPos - pos);
    result.path = requestTarget.substr(slashPos);

    return result;
}
