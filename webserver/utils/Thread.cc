/*
 * @Author: coxlong
 * @Date: 2021-04-12 18:38:49
 * @LastEditTime: 2021-04-12 22:44:42
 */
#include <assert.h>
#include <glog/logging.h>
#include <webserver/utils/Thread.h>

using namespace webserver;

namespace webserver {
namespace CurrentThread {
__thread pthread_t cachedTid = 0;
}
}

Thread::Thread(ThreadFunc threadFunc)
    : func(threadFunc),
      tid(0),
      started(false),
      joined(false) {
}

Thread::~Thread() {
    if(started && !joined) {
        pthread_detach(tid);
    }
}

void* callFunc(void* funcP) {
    auto func = *reinterpret_cast<ThreadFunc*>(funcP);
    func();
    return nullptr;
}

void Thread::start() {
    assert(!started);
    started = true;
    if(pthread_create(&tid, nullptr, &callFunc, reinterpret_cast<void*>(&func))) {
        started = false;
        LOG(FATAL) << "pthread_create failure";
    }
}

int Thread::join() {
    assert(started);
    assert(!joined);
    joined = true;
    return pthread_join(tid, nullptr);
}