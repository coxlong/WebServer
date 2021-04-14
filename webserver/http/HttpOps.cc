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

void sendResponse(int connfd, const std::string& msg) {

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
            HttpResponse response;
            std::string msg("helloworld");
            response.setContext(msg);
            response.setHeader("Content-Length", std::to_string(msg.size()));
            response.setStatus(HttpStatusCode::S200);
            sendMsg(connFd, response.toMsg());
        }
    } else {
        LOG(ERROR) << "channelWeakPtr is expired";
    }
}