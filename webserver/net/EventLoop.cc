/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:09:59
 * @LastEditTime: 2021-04-12 23:12:11
 */
#include <sys/eventfd.h>

#include <webserver/net/EventLoop.h>
#include <webserver/net/Epoll.h>
#include <webserver/net/Channel.h>
#include <webserver/net/SocketUtils.h>

using namespace webserver;
using namespace webserver::net;

__thread EventLoop* t_eventLoop = nullptr;

EventLoop::EventLoop()
    : epollPtr(std::make_unique<Epoll>()),
      quited(false),
      eventHandling(false),
      threadId(CurrentThread::tid()),
      pendingFunctors(0),
      mutex(),
      callingPendingFunctors(false),
      wakeupFd(createEventFd()),
      wakeupChannel(std::make_shared<Channel>(this, wakeupFd)) {
    assert(t_eventLoop == nullptr);
    t_eventLoop = this;
    wakeupChannel->setReadCallback(std::bind(&EventLoop::handleWakeup, this));
    wakeupChannel->setEvents(EPOLLIN | EPOLLPRI);
    wakeupChannel->enableReading();
}

EventLoop::~EventLoop() {
    t_eventLoop = nullptr;
}

void EventLoop::updateChannel(ChannelPtr channelPtr) {
    assertInLoopThread();
    epollPtr->updateChannel(channelPtr);
}

void EventLoop::removeChannel(int fd) {
    assertInLoopThread();
    epollPtr->removeChannel(fd);
}

void EventLoop::loop() {
    while(!quited) {
        auto readyChannles = epollPtr->poll();
        eventHandling = true;
        for(auto channel:readyChannles) {
            channel->handleEvents();
        }
        doPendingFunctors();
        eventHandling = false;        
    }
}

void EventLoop::quit() {
    quited = true;
}

void EventLoop::runInLoop(Functor&& cb) {
    if(isInLoopThread()) {
        cb();
    } else {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functor&& cb) {
    {
        MutexLockGuard lock(mutex);
        pendingFunctors.emplace_back(std::move(cb));
    }
    if(!isInLoopThread() || callingPendingFunctors) {
        wakeup();
    }
}

int EventLoop::getChannelSize() {
    return epollPtr->getChannelSize();
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors = true;
    {
        MutexLockGuard lock(mutex);
        functors.swap(pendingFunctors);
    }
    for(auto func:functors) {
        LOG(ERROR) << "call func()";
        func();
    }
    callingPendingFunctors = false;
}

void EventLoop::wakeup() {
    std::string one(8, '1');
    if(sendMsg(wakeupFd, one) != static_cast<ssize_t>(one.size())) {
        LOG(ERROR) << "wakeup failure";
    }
}

void EventLoop::handleWakeup() {
    std::string buf(8, '\0');
    LOG(ERROR) << "wakeupFd=" << wakeupFd << " len=" << read(wakeupFd, &buf[0], buf.size());
    // LOG(ERROR) << "wakeupFd=" << wakeupFd << " len=" << recvMsg(wakeupFd, buf);
}

int EventLoop::createEventFd() {
    int fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(fd < 0) {
        LOG(FATAL) << "createEventFd failed";
    }
    return fd;
}