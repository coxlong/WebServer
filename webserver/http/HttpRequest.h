/*
 * @Author: coxlong
 * @Date: 2021-04-13 20:44:10
 * @LastEditTime: 2021-04-13 23:16:13
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
    explicit HttpRequest(const std::string& buf)
        : method(Invalid),
          version(unknown),
          URL(),
          requestData(),
          headers() {
        auto i=buf.find("\r\n");
        if(i!=std::string::npos) {
            auto firstLine=buf.substr(0, i);
            std::regex ws_re(" ");
            std::vector<std::string> line(std::sregex_token_iterator(firstLine.begin(), firstLine.end(), ws_re, -1), std::sregex_token_iterator());
            if(line.size() == 3) {
                // 设置请求行
                if(line[0]=="GET") {
                    method = GET;
                } else {
                    return;
                }
                URL = line[1];
                if(line[2]=="HTTP/1.1") {
                    version = http11;
                } else {
                    method = Invalid;
                    return;
                }

                // 设置请求头部
                auto len = buf.size();
                i += 2;
                std::regex ws_re2(":");
                while(i<len) {
                    auto j=buf.find("\r\n", i);
                    if(j==i) {
                        // 设置请求数据
                        requestData = buf.substr(j+2);
                        break;
                    } else if(j==std::string::npos) {
                        method = Invalid;
                        return;
                    }
                    auto k=buf.find(":", i);
                    if(k>i && k<j) {
                        headers[buf.substr(i, k-i)] = buf.substr(k+1, j-k-1);
                    } else {
                        method = Invalid;
                        return;
                    }
                    i=j+2;
                }
            }
        }
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