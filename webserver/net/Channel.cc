/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:10:14
 * @LastEditTime: 2021-04-10 20:20:25
 */
#include <sys/epoll.h>

#include <webserver/net/Channel.h>
#include <webserver/net/EventLoop.h>

using namespace webserver;
using namespace webserver::net;

Channel::Channel(EventLoop* ownerLoop, int fd)
    : ownerLoop(ownerLoop),
      fd(fd),
      events(0),
      revents(0) {
}

Channel::~Channel() {}

void Channel::handleEvents() {
    if(revents&EPOLLERR) {
        if(errorCallback) {
            errorCallback();
        }
        return;
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