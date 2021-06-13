/*
 * @Author: coxlong
 * @Date: 2021-04-11 10:45:36
 * @LastEditTime: 2021-06-13 17:58:36
 */
#include <webserver/net/TCPServer.h>
#include <webserver/net/SocketUtils.h>
#include <webserver/net/EventLoop.h>
#include <webserver/net/Channel.h>
#include <webserver/net/EventLoopThreadPool.h>
#include <webserver/http/HttpOps.h>

using namespace webserver;
using namespace webserver::net;

TCPServer::TCPServer(std::shared_ptr<EventLoop> eventLoopPtr, const int threadNum)
    : listenFd(initSocket()),
      eventLoopPtr(eventLoopPtr),
      channelPtr(std::make_shared<Channel>(eventLoopPtr, listenFd)),
      threadNum(threadNum),
      connReadCallback(&webserver::http::handleRead),
      eventLoopThreadPool(std::make_unique<EventLoopThreadPool>(eventLoopPtr, threadNum)) {
}

TCPServer::~TCPServer() {
}

void TCPServer::start() {
    if(listenSocket(listenFd) == 0 ) {
        LOG(INFO) << "listening...";
    } else {
        LOG(FATAL) << "can't listen";
    }
    eventLoopThreadPool->start();
    channelPtr->setReadCallback(std::bind(&TCPServer::newConnection, this));
    channelPtr->enableReading();
}

void TCPServer::stop() {
    eventLoopThreadPool->stop();
    eventLoopPtr->quit();
}

void TCPServer::newConnection() {
    while(true) {
        auto connFd = acceptConn(listenFd);
        if(connFd < 0) {
            break;
        } else {
            auto nextLoop = eventLoopThreadPool->getNextLoop();
            // TODO
            auto channelPtr=std::make_shared<Channel>(nextLoop, connFd);
            // 不能传递强指针给回调函数，否则造成内存泄漏
            channelPtr->setReadCallback(std::bind(connReadCallback, ChannelWeakPtr(channelPtr)));
            channelPtr->setEpollET();
            nextLoop->queueInLoop(std::bind(&Channel::enableReading, channelPtr));
        }
    }
}