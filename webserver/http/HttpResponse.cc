
#include <glog/logging.h>

#include <webserver/http/HttpResponse.h>
#include <webserver/http/HttpRequest.h>

using namespace webserver;
using namespace webserver::http;

HttpResponse::HttpResponse()
    : status(S400),
      version(http11),
      headers() {
    headers["Date"] = getCurTime();
}

HttpResponse::~HttpResponse() {}

void HttpResponse::handleRequest(const HttpRequest& request) {
    // LOG(ERROR) << "method=" << request.method;
    if(request.method == GET) {
        parseURL(request.URL);
    } else if(request.method == POST) {
        // TODO
    }
}


std::string HttpResponse::toMsg() {
    std::string res=vMap[version] + SPACE + sCodes[status] + SPACE + sDescriptions[status] + CRLF;
    
    for(auto h:headers) {
        res += h.first + ":" + h.second + CRLF;
    }
    res += CRLF;

    res += context;
    // LOG(ERROR) << res;
    return res;
}


void HttpResponse::parseURL(const std::string& URL) {
    if(URL == "/favicon.ico") {
        status = S200;
        headers["Content-Type"] = cTypeMap.at(".ico");
        context = std::string(favicon, 555);
    } else {
        status = S200;
        context = "hello";
        headers["Content-Length"] = std::to_string(context.size());
    }
}

void HttpResponse::NotFound() {
    status = S404;
    context = "404 Not Found";
    headers["Content-Length"] = std::to_string(context.size());
}