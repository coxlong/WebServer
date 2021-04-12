/*
 * @Author: coxlong
 * @Date: 2021-04-12 18:38:49
 * @LastEditTime: 2021-04-12 22:42:50
 */
#pragma once

#include <functional>
#include <pthread.h>
#include <mutex>
#include <condition_variable>

#include <webserver/utils/NonCopyable.h>

namespace webserver {


namespace CurrentThread {
extern __thread pthread_t cachedTid;

inline pthread_t tid() {
    if(cachedTid==0) {
        cachedTid = pthread_self();
    }
    return cachedTid;
}
}

using ThreadFunc=std::function<void()>;

using MutexLockGuard = std::unique_lock<std::mutex>;
using Condition = std::condition_variable;
using Mutex = std::mutex;

class Thread : NonCopyable {
public:
    explicit Thread(ThreadFunc threadFunc);
    ~Thread();

    void start();
    int join();

    pthread_t getptid() {return tid;}
    bool isStarted() {return started;}
    
private:
    ThreadFunc func;
    pthread_t tid;
    bool started;
    bool joined;
};
}