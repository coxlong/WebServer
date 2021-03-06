/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:10:14
 * @LastEditTime: 2021-06-13 17:58:57
 */
#include <sys/epoll.h>

#include <webserver/net/Channel.h>
#include <webserver/net/EventLoop.h>
#include <webserver/net/SocketUtils.h>

using namespace webserver;
using namespace webserver::net;

Channel::Channel(std::shared_ptr<EventLoop> ownerLoopPtr, const int fd)
    : ownerLoopPtr(ownerLoopPtr),
      fd(fd),
      events(0),
      revents(0) {
}

Channel::~Channel() {
    if(closeConn(fd) != 0) {
        LOG(ERROR) << "closeConn failed";
    }
}

void Channel::handleEvents() {
    if(revents&EPOLLERR) {
        if(errorCallback) {
            errorCallback();
        }
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
void Channel::setEpollET() {
    events |= EPOLLET;
}

void Channel::enableReading() {
    events |= (EPOLLIN | EPOLLPRI);
    ownerLoopPtr->updateChannel(shared_from_this());
}

void Channel::enableWriting() {
    events |= EPOLLOUT;
    ownerLoopPtr->updateChannel(shared_from_this());
}