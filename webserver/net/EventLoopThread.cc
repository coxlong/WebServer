/*
 * @Author: coxlong
 * @Date: 2021-04-12 18:38:49
 * @LastEditTime: 2021-06-12 12:35:10
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
    if(eventLoop) {
        eventLoop->quit();
        thread.join();
    }
}

EventLoop* EventLoopThread::startLoop() {
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

void EventLoopThread::threadFunc() {
    EventLoop newLoop;
    {
        MutexLockGuard lock(mutex);
        eventLoop = &newLoop;
        condition.notify_one();
    }
    newLoop.loop();
    eventLoop = nullptr;
}
}
}