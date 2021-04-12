#pragma once
#include <functional>
#include <pthread.h>

#include <webserver/utils/NonCopyable.h>

namespace webserver {

using ThreadFunc=std::function<void()>;

class Thread : NonCopyable {
public:
    explicit Thread(ThreadFunc threadFunc);
    ~Thread();

    void start();
    int join();

    pthread_t getptid() {return ptid;}
private:
    ThreadFunc func;
    pthread_t ptid;
    bool started;
    bool joined;
};
}