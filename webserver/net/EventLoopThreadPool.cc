/*
 * @Author: coxlong
 * @Date: 2021-04-12 18:38:49
 * @LastEditTime: 2021-04-12 23:25:41
 */
#include <webserver/net/EventLoopThreadPool.h>
#include <webserver/net/EventLoopThread.h>
#include <glog/logging.h>

using namespace webserver;
using namespace webserver::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* mainLoop, const int threadNum)
    : mainLoop(mainLoop),
      threadNum(threadNum),
      next(0),
      loopVec(0),
      loopThreads(0) {    
}

EventLoopThreadPool::~EventLoopThreadPool() {}

void EventLoopThreadPool::start() {
    for(int i=0; i<threadNum; ++i) {
        LOG(INFO) <<"eventloopthreadpool::start" << i;
        auto tmp = std::make_shared<EventLoopThread>();
        loopThreads.push_back(tmp);
        loopVec.push_back(tmp->startLoop());
    }
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    next = (next+1) % threadNum;
    return loopVec[next];
}