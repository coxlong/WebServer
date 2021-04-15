/*
 * @Author: coxlong
 * @Date: 2021-04-15 18:46:10
 * @LastEditTime: 2021-04-15 18:47:06
 */
#pragma once

#include <webserver/utils/NonCopyable.h>
#include <webserver/http/HttpUtils.h>

namespace webserver {
namespace http {

class HttpRequest;

class HttpResponse : NonCopyable {
public:
    HttpResponse();
    ~HttpResponse();
    void setStatus(HttpStatusCode statusCode) { status=statusCode; }
    void setHeader(const std::string& key, const std::string& value) { headers[key]=value; }
    void setContext(const std::string& context) { this->context = context; }

    void handleRequest(const HttpRequest& request);

    std::string toMsg();
private:
    void parseURL(std::string URL);
    void NotFound();

private:
    HttpStatusCode status;
    Version version;
    SSMap headers;
    std::string context;
};
}
}