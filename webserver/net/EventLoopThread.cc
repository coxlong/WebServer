/*
 * @Author: coxlong
 * @Date: 2021-04-12 18:38:49
 * @LastEditTime: 2021-06-13 10:23:53
 */
#include <webserver/net/EventLoopThread.h>
#include <webserver/net/EventLoop.h>

using namespace webserver;
using namespace webserver::net;

namespace webserver {
namespace net {

EventLoopThread::EventLoopThread()
    : eventLoop(nullptr),
      thread(std::bind(&EventLoopThread::threadFunc, this)),
      mutex(),
      condition() {
}

EventLoopThread::~EventLoopThread() {
}

std::shared_ptr<EventLoop> EventLoopThread::startLoop() {
    assert(!thread.isStarted());
    thread.start();
    {
        MutexLockGuard lock(mutex);
        while(eventLoop==nullptr) {
            condition.wait(lock);
        }
    }
    return eventLoop;
}

void EventLoopThread::quitLoop() {
    eventLoop->quit();
    eventLoop = nullptr;
}

void EventLoopThread::join() {
    thread.join();
}

void EventLoopThread::threadFunc() {
    std::shared_ptr<EventLoop> newLoopPtr = std::make_shared<EventLoop>();
    newLoopPtr->init();
    {
        MutexLockGuard lock(mutex);
        eventLoop = newLoopPtr;
        condition.notify_one();
    }
    newLoopPtr->loop();
}
}
}