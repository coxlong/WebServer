/*
 * @Author: coxlong
 * @Date: 2021-04-13 18:32:10
 * @LastEditTime: 2021-04-13 23:17:41
 */

#include <glog/logging.h>

#include <webserver/http/HttpOps.h>
#include <webserver/http/HttpRequest.h>
#include <webserver/net/Channel.h>
#include <webserver/net/EventLoop.h>
#include <webserver/net/SocketUtils.h>

using namespace webserver;
using namespace webserver::net;

void response(int connfd) {
    std::string msg = "HTTP/1.1 200 ok\r\n"
"Date: Sun, 17 Mar 2013 08:12:54 GM\r\n"
"Server: WebServer\r\n"
"Pragma: no-cache\r\n"
"Content-Length: 10\r\n"
"Keep-Alive: timeout=5, max=100\r\n"
"Connection: Keep-Alive\r\n"
"Content-Type: text/html; charset=utf-8\r\n"
"\r\n"
"helloworld";

    sendMsg(connfd, msg);
    // closeConn(connfd);

}

void webserver::http::handleRead(ChannelWeakPtr channelWeakPtr) {
    auto channelPtr = channelWeakPtr.lock();
    if(channelPtr) {
        auto connFd = channelPtr->getFd();
        auto loop = channelPtr->getOwnerLoop();
        LOG(ERROR) << "connfd=" << connFd;
        char buf[512];
        ssize_t len=0;
        if((len=recvMsg(connFd, buf, 512)) <= 0) {
            // 对方已断开
            loop->queueInLoop(std::bind(&EventLoop::removeChannel, loop, connFd));
        } else {
            std::string s(buf);
            HttpRequest request(s);
            LOG(ERROR) << request.method;
            response(connFd);
        }
    } else {
        LOG(ERROR) << "channelWeakPtr is expired";
    }
}