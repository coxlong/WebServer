/*
 * @Author: coxlong
 * @Date: 2021-04-12 18:38:49
 * @LastEditTime: 2021-06-13 10:24:26
 */
#include <webserver/net/EventLoopThreadPool.h>
#include <webserver/net/EventLoopThread.h>
#include <glog/logging.h>

using namespace webserver;
using namespace webserver::net;

EventLoopThreadPool::EventLoopThreadPool(std::shared_ptr<EventLoop> mainLoopPtr, const int threadNum)
    : mainLoopPtr(mainLoopPtr),
      threadNum(threadNum),
      next(0),
      loopVec(0),
      loopThreads(0) {
}

EventLoopThreadPool::~EventLoopThreadPool() {
}

void EventLoopThreadPool::start() {
    for(int i=0; i<threadNum; ++i) {
        auto tmp = std::make_shared<EventLoopThread>();
        loopThreads.push_back(tmp);
        loopVec.push_back(tmp->startLoop());
    }
}

void EventLoopThreadPool::stop() {
    for(int i=0; i<threadNum; ++i) {
        loopVec[i] = nullptr;
        loopThreads[i]->quitLoop();
        loopThreads[i]->join();
    }
}

std::shared_ptr<EventLoop> EventLoopThreadPool::getNextLoop() {
    next = (next+1) % threadNum;
    return loopVec[next];
}