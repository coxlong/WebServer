/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:09:51
 * @LastEditTime: 2021-04-12 23:19:39
 */
#pragma once
#include <memory>
#include <assert.h>

#include <glog/logging.h>

#include <webserver/utils/NonCopyable.h>
#include <webserver/utils/Thread.h>

namespace webserver {
namespace net {

class Epoll;
class Channel;
using ChannelPtr=std::shared_ptr<Channel>;
using Functor=std::function<void()>;

class EventLoop : NonCopyable {
public:
    EventLoop();
    ~EventLoop();

    void updateChannel(ChannelPtr channelPtr);
    void removeChannel(int fd);

    void loop();
    void quit();

    bool isInLoopThread() const { return threadId==CurrentThread::tid(); }
    void assertInLoopThread() const { assert(isInLoopThread()); }

    void runInLoop(Functor&& cb);
    void queueInLoop(Functor&& cb);

    const int getTid() const { return threadId; }

    int getChannelSize();
private:
    void doPendingFunctors();
    void wakeup();

private:
    std::unique_ptr<Epoll> epollPtr;
    bool quited;
    bool eventHandling;
    const pthread_t threadId;
    std::vector<Functor> pendingFunctors;
    Mutex mutex;
    bool callingPendingFunctors;
};
}
}