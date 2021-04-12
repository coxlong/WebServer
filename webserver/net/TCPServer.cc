/*
 * @Author: coxlong
 * @Date: 2021-04-11 10:45:36
 * @LastEditTime: 2021-04-12 23:20:49
 */
#include <iostream>

#include <webserver/net/TCPServer.h>
#include <webserver/net/SocketUtils.h>
#include <webserver/net/EventLoop.h>
#include <webserver/net/Channel.h>
#include <webserver/net/EventLoopThreadPool.h>

using namespace webserver;
using namespace webserver::net;

TCPServer::TCPServer(EventLoop* eventLoop, const int threadNum)
    : listenFd(initSocket()),
      eventLoop(eventLoop),
      channelPtr(std::make_shared<Channel>(eventLoop, listenFd)),
      threadNum(threadNum),
      eventLoopThreadPool(std::make_unique<EventLoopThreadPool>(eventLoop, threadNum)) {
}

TCPServer::~TCPServer() {}

void TCPServer::start() {
    if(listenSocket(listenFd) == 0 ) {
        LOG(INFO) << "listening...";
    } else {
        LOG(FATAL) << "can't listen";
    }
    channelPtr->setReadCallback(std::bind(&TCPServer::newConnection, this));
    channelPtr->enableReading();
}

void TCPServer::newConnection() {
    auto connFd = acceptConn(listenFd);
    if(connFd < 0) {
        LOG(ERROR) << "acceptConn error!";
    } else {
        LOG(INFO) << "new connection; connFd=" << connFd;
        auto nextLoop = eventLoopThreadPool->getNextLoop();
        auto channelPtr=std::make_shared<Channel>(nextLoop, connFd);
        // channelPtr->setWriteCallback(std::bind(sendMsg, connFd, "hello\n"));
        // channelPtr->enableWriting();
        channelPtr->setReadCallback(std::bind(&TCPServer::handleRead, this, connFd, nextLoop->getTid()));
        // channelPtr->enableReading();
        nextLoop->queueInLoop(std::bind(&Channel::enableReading, channelPtr));
        //sendMsg(connFd, "helloword!\n");
        //close(connFd);
    }
}

void TCPServer::handleRead(int connFd, int loopTid) {
    std::cerr << "loopTid=" << loopTid << std::endl;
    char buf[512];
    ssize_t len=0;
    if((len=recvMsg(connFd, buf, 512)) <= 0) {
        eventLoop->removeChannel(connFd);
    } else {
        buf[len] = '\0';
        std::cerr << buf;
    }
}