/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:10:14
 * @LastEditTime: 2021-04-11 21:41:38
 */
#include <sys/epoll.h>

#include <iostream>

#include <webserver/net/Channel.h>
#include <webserver/net/EventLoop.h>

using namespace webserver;
using namespace webserver::net;

Channel::Channel(EventLoop* ownerLoop, const int fd)
    : ownerLoop(ownerLoop),
      fd(fd),
      events(0),
      revents(0) {
}

Channel::~Channel() {}

void Channel::handleEvents() {
    std::cerr << "channelSize: " << ownerLoop->getChannelSize() << std::endl;
    if(revents&EPOLLERR) {
        if(errorCallback) {
            errorCallback();
        }
        // LOG(INFO) << "delete1 Channel";
        // std::cerr << "error";
        // ownerLoop->removeChannel(shared_from_this());
        // return;
    }
    if(revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
        if(readCallback) {
            readCallback();
        }
    }
    if(revents & EPOLLOUT) {
        if(writeCallback) {
            writeCallback();
        }
    }
}

void Channel::enableReading() {
    events |= (EPOLLIN | EPOLLPRI);
    ownerLoop->updateChannel(shared_from_this());
}

void Channel::enableWriting() {
    events |= EPOLLOUT;
    ownerLoop->updateChannel(shared_from_this());
}