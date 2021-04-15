/*
 * @Author: coxlong
 * @Date: 2021-04-15 18:46:10
 * @LastEditTime: 2021-04-15 18:46:59
 */
#include <fstream>

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
    headers["Server"] = "WebServer/1.0";
}

HttpResponse::~HttpResponse() {}

void HttpResponse::handleRequest(const HttpRequest& request) {
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

void HttpResponse::parseURL(std::string URL) {
    if(URL == "/favicon.ico") {
        status = S200;
        headers["Content-Type"] = cTypeMap.at(".ico");
        context = std::string(favicon, 555);
        return;
    }
    if(*URL.rbegin() == '/') {
        URL += "index.html";
    }
    auto it=URL.rfind(".");
    if(it == std::string::npos) {
        NotFound();
        return;
    }
    auto fileType = URL.substr(it);
    LOG(ERROR) << "fileType=" << fileType;
    auto filename = wwwroot+URL;
    LOG(ERROR) << "filename=" << filename;
    if(std::ifstream is{filename, std::ios::binary | std::ios::ate}) {
        status = S200;
        auto size=is.tellg();
        LOG(ERROR) << "size=" << size;
        std::string str(size, '\0');
        is.seekg(0);
        if(!is.read(&str[0], size)) {
            NotFound();
            return;
        }
        str.swap(context);
        headers["Content-Length"] = std::to_string(context.size());
        if(cTypeMap.find(fileType)!=cTypeMap.end()) {
            headers["Content-Type"] = cTypeMap[fileType];
        }
    } else {
        LOG(ERROR) << "can't open the file";
        NotFound();
    }
}

void HttpResponse::NotFound() {
    status = S404;
    context = "404 Not Found";
    headers["Content-Length"] = std::to_string(context.size());
}