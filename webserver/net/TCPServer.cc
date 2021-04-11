/*
 * @Author: coxlong
 * @Date: 2021-04-11 10:45:36
 * @LastEditTime: 2021-04-11 22:04:11
 */
#include <iostream>

#include <webserver/net/TCPServer.h>
#include <webserver/net/SocketUtils.h>
#include <webserver/net/EventLoop.h>
#include <webserver/net/Channel.h>

using namespace webserver;
using namespace webserver::net;

TCPServer::TCPServer(EventLoop* eventLoop)
    : listenFd(initSocket()),
      eventLoop(eventLoop),
      channelPtr(std::make_shared<Channel>(eventLoop, listenFd)) {    
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
        LOG(INFO) << "connFd=" << connFd;
        auto channelPtr=std::make_shared<Channel>(eventLoop, connFd);
        // channelPtr->setWriteCallback(std::bind(sendMsg, connFd, "hello\n"));
        // channelPtr->enableWriting();
        channelPtr->setReadCallback(std::bind(&TCPServer::handleRead, this, connFd));
        channelPtr->enableReading();
        //sendMsg(connFd, "helloword!\n");
        //close(connFd);
    }
}

void TCPServer::handleRead(int connFd) {
    char buf[512];
    ssize_t len=0;
    if((len=recvMsg(connFd, buf, 512)) <= 0) {
        eventLoop->removeChannel(connFd);
    } else {
        buf[len] = '\0';
        std::cerr << buf;
    }
}