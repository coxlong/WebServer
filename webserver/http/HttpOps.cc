/*
 * @Author: coxlong
 * @Date: 2021-04-13 18:32:10
 * @LastEditTime: 2021-04-15 21:55:04
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
        LOG(ERROR) << "connfd=" << connFd;
        auto loop = channelPtr->getOwnerLoop();
        HttpRequest request;
        if(request.init(connFd)) {            
            HttpResponse response;
            response.handleRequest(request);
            sendMsg(connFd, response.toMsg());
        } else {            
            loop->queueInLoop(std::bind(&EventLoop::removeChannel, loop, connFd));
        }
    } else {
        LOG(ERROR) << "channelWeakPtr is expired";
    }
}