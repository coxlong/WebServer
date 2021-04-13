/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:09:59
 * @LastEditTime: 2021-04-12 23:12:11
 */
#include <webserver/net/EventLoop.h>
#include <webserver/net/Epoll.h>
#include <webserver/net/Channel.h>

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
      callingPendingFunctors(false) {
    assert(t_eventLoop == nullptr);
    t_eventLoop = this;
}

EventLoop::~EventLoop() {
    t_eventLoop = nullptr;
}

void EventLoop::updateChannel(ChannelPtr channelPtr) {
    assertInLoopThread();
    epollPtr->updateChannel(channelPtr);
}

void EventLoop::removeChannel(int fd) {
    LOG(ERROR) << "fd=" << fd;
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
    LOG(ERROR) << "call queueInLoop";
    {
        MutexLockGuard lock(mutex);
        pendingFunctors.emplace_back(std::move(cb));
        LOG(ERROR) << "pendingFunctors size=" << pendingFunctors.size();
    }
    if(!isInLoopThread() || callingPendingFunctors) {
        LOG(ERROR) << "call wakeup";
        wakeup();
    }
}

int EventLoop::getChannelSize() {
    return epollPtr->getChannelSize();
}

void EventLoop::doPendingFunctors() {
    // LOG(ERROR) << "call doPendingFunctors";
    std::vector<Functor> functors;
    callingPendingFunctors = true;
    {
        MutexLockGuard lock(mutex);
        // LOG(ERROR) << "dopendingFunctors size=" << pendingFunctors.size();
        functors.swap(pendingFunctors);
        LOG(ERROR) << "functors size=" << functors.size();
        // LOG(ERROR) << "dopendingFunctors size=" << pendingFunctors.size();
    }
    for(auto func:functors) {
        func();
    }    
    callingPendingFunctors = false;
}

void EventLoop::wakeup() {
    // TODO
}