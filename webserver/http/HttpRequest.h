/*
 * @Author: coxlong
 * @Date: 2021-04-13 20:44:10
 * @LastEditTime: 2021-04-15 21:43:33
 */
#include <string>
#include <regex>
#include <unordered_map>

#include <glog/logging.h>

#include <webserver/utils/NonCopyable.h>
#include <webserver/http/HttpUtils.h>

namespace webserver {
namespace http {
class HttpRequest : NonCopyable {
public:

public:
    HttpRequest();
    bool init(int fd);

    void setRequestData(std::string&& data) {
        requestData = data;
    }

public:
    Method method;
    Version version;
    std::string URL;
    std::string requestData;
    SSMap headers;
};
}
}