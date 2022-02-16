#ifndef HTTP_PROXY_UTILS_H
#define HTTP_PROXY_UTILS_H

#include <boost/system/error_code.hpp>

/**
 * Обрабатывает err: если ошибка есть, печатает и завершает программу
 * @param err - объект ошибки
 */
void handleErr(const boost::system::error_code &err, const std::string &message);

#endif //HTTP_PROXY_UTILS_H
