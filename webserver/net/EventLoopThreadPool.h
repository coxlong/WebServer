/*
 * @Author: coxlong
 * @Date: 2021-04-12 18:38:49
 * @LastEditTime: 2021-04-12 21:00:20
 */
#pragma once
#include <vector>
#include <memory>

#include <webserver/utils/NonCopyable.h>

namespace webserver {
namespace net {

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : NonCopyable {
public:
    explicit EventLoopThreadPool(EventLoop* mainLoop, const int threadNum=5);
    ~EventLoopThreadPool();

    void start();

    EventLoop* getNextLoop();
private:
    EventLoop* mainLoop;
    const int threadNum;
    int next;
    std::vector<EventLoop*> loopVec;
    std::vector<std::shared_ptr<EventLoopThread>> loopThreads;
};
}
}