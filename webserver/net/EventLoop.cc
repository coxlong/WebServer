/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:09:59
 * @LastEditTime: 2021-06-12 18:07:58
 */
#include <sys/eventfd.h>

#include <webserver/net/EventLoop.h>
#include <webserver/net/Epoll.h>
#include <webserver/net/Channel.h>
#include <webserver/net/SocketUtils.h>
#include <webserver/utils/Allocator.h>

using namespace webserver;
using namespace webserver::net;

__thread EventLoop* t_eventLoop = nullptr;
__thread MemPool*   webserver::t_memPoolPtr = nullptr;

EventLoop::EventLoop()
    : epollPtr(std::make_unique<Epoll>()),
      quited(false),
      eventHandling(false),
      threadId(CurrentThread::tid()),
      pendingFunctors(0),
      mutex(),
      callingPendingFunctors(false),
      wakeupFd(createEventFd()),
      wakeupChannel(std::make_shared<Channel>(this, wakeupFd)),
      wBuf(8, '\0') {
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
    std::vector<ChannelPtr, Alloc<ChannelPtr>> readyChannelPtr;
    while(!quited) {
        readyChannelPtr.clear();
        epollPtr->poll(readyChannelPtr);
        eventHandling = true;
        for(auto& channel:readyChannelPtr) {
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
    decltype(pendingFunctors) functors;
    callingPendingFunctors = true;
    {
        MutexLockGuard lock(mutex);
        functors.swap(pendingFunctors);
    }
    for(auto& func:functors) {
        func();
    }
    callingPendingFunctors = false;
}

void EventLoop::wakeup() {
    static const std::string one(8, '1');
    ssize_t ret;
    if((ret=write(wakeupFd, one.data(), 8)) < 0) {
        LOG(WARNING) << strerror(errno);
    }
}

void EventLoop::handleWakeup() {
    ssize_t ret;
    if((ret=read(wakeupFd, &wBuf[0], 8)) < 0) {
        LOG(ERROR) << strerror(errno);
    }
}

int EventLoop::createEventFd() {
    int fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(fd < 0) {
        LOG(FATAL) << "createEventFd failed";
    }
    return fd;
}