/*
 * @Author: coxlong
 * @Date: 2021-04-12 18:38:49
 * @LastEditTime: 2021-04-12 21:45:21
 */
#pragma once

#include <webserver/utils/NonCopyable.h>
#include <webserver/utils/Thread.h>

namespace webserver {
namespace net {

class EventLoop;

class EventLoopThread : NonCopyable {
public:
    EventLoopThread();
    ~EventLoopThread();

    EventLoop* startLoop();

private:
    void threadFunc();

private:
    EventLoop* eventLoop;
    Thread thread;
    Mutex mutex;
    Condition condition;
};
}
}