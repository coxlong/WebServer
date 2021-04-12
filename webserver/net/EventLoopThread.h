#pragma once

#include <webserver/utils/NonCopyable.h>

namespace webserver {
namespace net {

class EventLoopThread : NonCopyable {
public:
    EventLoopThread();
    ~EventLoopThread();
private:
};

}
}