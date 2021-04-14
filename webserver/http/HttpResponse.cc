
#include <glog/logging.h>

#include <webserver/http/HttpResponse.h>

using namespace webserver;
using namespace webserver::http;

HttpResponse::HttpResponse()
    : status(S400),
      version(http11),
      headers() {
}

HttpResponse::~HttpResponse() {}

std::string HttpResponse::toMsg() {
    std::string res=vMap[version] + SPACE + sCodes[status] + SPACE + sDescriptions[status] + CRLF;
    
    for(auto h:headers) {
        res += h.first + ":" + h.second + CRLF;
    }
    res += CRLF;

    res += context;
    LOG(ERROR) << res;
    return res;
}

