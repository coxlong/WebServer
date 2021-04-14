/*
 * @Author: coxlong
 * @Date: 2021-04-13 18:32:10
 * @LastEditTime: 2021-04-13 23:17:41
 */

#include <glog/logging.h>

#include <webserver/http/HttpOps.h>
#include <webserver/http/HttpRequest.h>
#include <webserver/http/HttpResponse.h>
#include <webserver/net/Channel.h>
#include <webserver/net/EventLoop.h>
#include <webserver/net/SocketUtils.h>

using namespace webserver;
using namespace webserver::net;

void http::handleRead(ChannelWeakPtr channelWeakPtr) {
    auto channelPtr = channelWeakPtr.lock();
    if(channelPtr) {
        auto connFd = channelPtr->getFd();
        auto loop = channelPtr->getOwnerLoop();
        LOG(ERROR) << "connfd=" << connFd;
        // char buf[4096];
        std::string buf;
        ssize_t len=0;
        if((len=recvMsg(connFd, buf)) <= 0) {
            // 对方已断开
            loop->queueInLoop(std::bind(&EventLoop::removeChannel, loop, connFd));
        } else {
            HttpRequest request(buf);
            // LOG(ERROR) << buf;
            LOG(ERROR) << "method=" << request.method;
            if(request.method==Invalid) {
                LOG(ERROR) << buf;
            }
            // len = recvMsg(connFd, buf, 4096);
            // request.setRequestData(std::string(buf, len));
            
            HttpResponse response;
            response.handleRequest(request);
            sendMsg(connFd, response.toMsg());
            // loop->queueInLoop(std::bind(&EventLoop::removeChannel, loop, connFd));
        }
    } else {
        LOG(ERROR) << "channelWeakPtr is expired";
    }
}