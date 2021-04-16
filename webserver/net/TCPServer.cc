/*
 * @Author: coxlong
 * @Date: 2021-04-11 10:45:36
 * @LastEditTime: 2021-04-12 23:20:49
 */


#include <webserver/net/TCPServer.h>
#include <webserver/net/SocketUtils.h>
#include <webserver/net/EventLoop.h>
#include <webserver/net/Channel.h>
#include <webserver/net/EventLoopThreadPool.h>
#include <webserver/http/HttpOps.h>

using namespace webserver;
using namespace webserver::net;

TCPServer::TCPServer(EventLoop* eventLoop, const int threadNum)
    : listenFd(initSocket()),
      eventLoop(eventLoop),
      channelPtr(std::make_shared<Channel>(eventLoop, listenFd)),
      threadNum(threadNum),
      connReadCallback(&webserver::http::handleRead),
      eventLoopThreadPool(std::make_unique<EventLoopThreadPool>(eventLoop, threadNum)) {
}

TCPServer::~TCPServer() {}

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

void TCPServer::newConnection() {
    std::string peerInfo;
    auto connFd = acceptConn(listenFd, peerInfo);
    if(connFd < 0) {
        LOG(ERROR) << "acceptConn error!";
    } else {
        LOG(ERROR) << "new connection; peer is " << peerInfo;
        auto nextLoop = eventLoopThreadPool->getNextLoop();
        auto channelPtr=std::make_shared<Channel>(nextLoop, connFd);
        // 不能传递强指针给回调函数，否则造成内存泄漏
        channelPtr->setReadCallback(std::bind(connReadCallback, ChannelWeakPtr(channelPtr)));
        nextLoop->queueInLoop(std::bind(&Channel::enableReading, channelPtr));
    }
}