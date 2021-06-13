/*
 * @Author: coxlong
 * @Date: 2021-04-12 18:38:49
 * @LastEditTime: 2021-06-13 10:20:32
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
    explicit EventLoopThreadPool(std::shared_ptr<EventLoop> mainLoopPtr, const int threadNum=5);
    ~EventLoopThreadPool();

    void start();
    void stop();

    std::shared_ptr<EventLoop> getNextLoop();
private:
    std::shared_ptr<EventLoop> mainLoopPtr;
    const int threadNum;
    int next;
    std::vector<std::shared_ptr<EventLoop>> loopVec;
    std::vector<std::shared_ptr<EventLoopThread>> loopThreads;
};
}
}