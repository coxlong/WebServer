/*
 * @Author: coxlong
 * @Date: 2021-04-12 18:38:49
 * @LastEditTime: 2021-06-13 10:25:41
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

    std::shared_ptr<EventLoop> startLoop();
    void quitLoop();
    void join();

private:
    void threadFunc();

private:
    std::shared_ptr<EventLoop> eventLoop;
    Thread thread;
    Mutex mutex;
    Condition condition;
};
}
}