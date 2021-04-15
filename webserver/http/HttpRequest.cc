/*
 * @Author: coxlong
 * @Date: 2021-04-15 19:32:03
 * @LastEditTime: 2021-04-15 22:02:43
 */
#include <sstream>
#include <algorithm>

#include <webserver/http/HttpRequest.h>
#include <webserver/net/SocketUtils.h>

using namespace webserver;
using namespace webserver::net;
using namespace webserver::http;

HttpRequest::HttpRequest()
    : method(Invalid),
        version(unknown),
        URL(),
        requestData(),
        headers() {
}

bool HttpRequest::init(int fd) {
    std::string rbuf(4096, '\0');
    auto len=recvMsg(fd, rbuf);
    if(len <= 0) {
        return false;
    }

    std::string buf(rbuf.substr(0, len));

    auto i=buf.find(CRLF);
    if(i!=std::string::npos) {
        auto firstLine=buf.substr(0, i);
        std::regex ws_re(SPACE);
        std::vector<std::string> line(std::sregex_token_iterator(firstLine.begin(), firstLine.end(), ws_re, -1), std::sregex_token_iterator());
        if(line.size() == 3) {
            // 设置请求行
            if(line[0]=="GET") {
                method = GET;
            } else {
                return false;
            }
            URL = line[1];
            if(line[2]==vMap[http11]) {
                version = http11;
            } else {
                return false;
            }

            // 设置请求头部
            i += 2;
            std::regex ws_re2(COLON);
            int content_length = 0;
            while(true) {
                while(i>=buf.size()) {
                    // 继续读取
                    len=recvMsg(fd, rbuf);
                    buf.append(rbuf.substr(0, len));
                }
                auto j=buf.find(CRLF, i);
                if(j==i) {
                    // 设置请求数据
                    if(headers.find("content-length")!=headers.end()) {
                        content_length = std::stoi(headers["content-length"]);
                    }
                    requestData = buf.substr(j+2);
                    while(requestData.size()<static_cast<size_t>(content_length)) {
                        len=recvMsg(fd, rbuf);
                        requestData.append(rbuf.substr(0, len));
                    }
                    return true;
                } else if(j==std::string::npos) {
                    // 继续从socket读入
                    continue;
                }
                auto k=buf.find(COLON, i);
                if(k>i && k<j) {
                    std::string tmp=buf.substr(i, k-i);
                    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
                    headers[tmp] = buf.substr(k+1, j-k-1);
                } else {
                    return false;
                }
                i=j+2;
            }
        }
    }
    return false;
}