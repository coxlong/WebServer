
#include <glog/logging.h>

#include <webserver/http/HttpOps.h>
#include <webserver/net/Channel.h>
#include <webserver/net/EventLoop.h>
#include <webserver/net/SocketUtils.h>

using namespace webserver;
using namespace webserver::net;

void response(int connfd) {
    std::string msg = "HTTP/1.1 200 OK\r\n\
Date: Sun, 17 Mar 2013 08:12:54 GM\r\n\
Server: Apache/2.2.8 (Win32) PHP/5.2.5\r\n\
Pragma: no-cache\r\n\
Content-Length: 4393\r\n\
Keep-Alive: timeout=5, max=100\r\n\
Connection: Keep-Alive\r\n\
Content-Type: text/html; charset=utf-8\r\n\
\n\r \
<html>\r\n \
<head>\
<title>HTTP响应示例<title> \
</head> \
<body> \
Hello HTTP! \
</body> \
</html>";

    LOG(ERROR) << "sendsize=" << sendMsg(connfd, msg);
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
            LOG(ERROR) << "len=" << len;
            
            loop->queueInLoop(std::bind(&EventLoop::removeChannel, loop, connFd));
        } else {
            buf[len] = '\0';
            LOG(INFO) << "connfd=" << connFd;
            LOG(INFO) << buf;
            LOG(ERROR) << strtok(buf, "\n");
            response(connFd);
        }
    } else {
        LOG(ERROR) << "channelWeakPtr is expired";
    }
}