/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:09:51
 * @LastEditTime: 2021-06-13 18:40:23
 */
#pragma once
#include <memory>
#include <assert.h>

#include <glog/logging.h>

#include <webserver/utils/NonCopyable.h>
#include <webserver/utils/Thread.h>
#include <webserver/utils/Allocator.h>

namespace webserver {
namespace net {

class Epoll;
class Channel;
using ChannelPtr=std::shared_ptr<Channel>;
using ChannelWeakPtr=std::weak_ptr<Channel>;
using Functor=std::function<void()>;

class EventLoop : NonCopyable, public std::enable_shared_from_this<EventLoop> {
public:
    EventLoop();
    ~EventLoop();

    void init();

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
    void handleWakeup();
    int createEventFd();

private:
    MemPoolRes memPoolRes;              // 内存池必须最先构造
    std::unique_ptr<Epoll> epollPtr;
    bool quited;
    bool eventHandling;
    const pthread_t threadId;
    std::vector<Functor, Alloc<Functor>> pendingFunctors;
    Mutex mutex;
    bool callingPendingFunctors;
    const int wakeupFd;
    std::string wBuf;
};
}
}