#pragma once


#include <webserver/utils/NonCopyable.h>
#include <webserver/http/HttpUtils.h>

namespace webserver {
namespace http {


class HttpResponse : NonCopyable {
public:
    HttpResponse();
    ~HttpResponse();
    void setStatus(HttpStatusCode statusCode) { status=statusCode; }
    void setHeader(const std::string& key, const std::string& value) { headers[key]=value; }
    void setContext(const std::string& context) { this->context = context; }

    std::string toMsg();

private:
    HttpStatusCode status;
    Version version;
    SSMap headers;
    std::string context;
};
}
}