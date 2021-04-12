#include <assert.h>
#include <glog/logging.h>
#include <webserver/utils/Thread.h>

using namespace webserver;

Thread::Thread(ThreadFunc threadFunc)
    : func(threadFunc),
      ptid(0),
      started(false),
      joined(false) {
}

Thread::~Thread() {
    if(started && !joined) {
        pthread_detach(ptid);
    }
}

void* callFunc(void* funcP) {
    auto func = *reinterpret_cast<ThreadFunc*>(funcP);
    func();
}

void Thread::start() {
    assert(!started);
    started = true;
    if(pthread_create(&ptid, nullptr, &callFunc, reinterpret_cast<void*>(&func))) {
        started = false;
        LOG(FATAL) << "pthread_create failiure";
    }
}

int Thread::join() {
    assert(started);
    assert(!joined);
    joined = true;
    return pthread_join(ptid, nullptr);
}