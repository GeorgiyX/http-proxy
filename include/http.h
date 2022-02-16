#ifndef HTTP_PROXY_HTTP_H
#define HTTP_PROXY_HTTP_H

#include <string>

extern const std::string HTTP1_1;
extern const std::string PROXY_CONNECTION;

struct HostPortPath {
    static HostPortPath fromRequestTarget(const std::string &requestTarget);
    std::string host;
    unsigned port;
    std::string path;
};

#endif //HTTP_PROXY_HTTP_H
