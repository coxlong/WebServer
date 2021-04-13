
#include <glog/logging.h>

#include <webserver/http/HttpOps.h>
#include <webserver/net/Channel.h>
#include <webserver/net/EventLoop.h>
#include <webserver/net/SocketUtils.h>

using namespace webserver;
using namespace webserver::net;

void webserver::http::handleRead(ChannelWeakPtr channelWeakPtr) {
    auto channelPtr = channelWeakPtr.lock();
    if(channelPtr) {
        auto connFd = channelPtr->getFd();
        auto loop = channelPtr->getOwnerLoop();
        LOG(ERROR) << "connfd=" << connFd;
        // LOG(ERROR) << "loopTid=" << loopTid;
        char buf[512];
        ssize_t len=0;
        if((len=recvMsg(connFd, buf, 512)) <= 0) {
            LOG(ERROR) << "len=" << len;
            
            loop->queueInLoop(std::bind(&EventLoop::removeChannel, loop, connFd));
            // eventLoop->removeChannel(connFd);
        } else {
            buf[len] = '\0';
            LOG(ERROR) << buf;
        }
    } else {
        LOG(ERROR) << "channelWeakPtr is expired";
    }
}