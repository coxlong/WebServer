/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:09:51
 * @LastEditTime: 2021-04-10 19:01:57
 */
#pragma once
#include <memory>
#include <assert.h>

#include <glog/logging.h>

#include <webserver/utils/NonCopyable.h>

namespace webserver {
namespace net {

class Epoll;
class Channel;
using ChannelPtr=std::shared_ptr<Channel>;

class EventLoop : NonCopyable {
public:
    EventLoop();
    ~EventLoop();

    void updateChannel(ChannelPtr channelPtr);
    void removeChannel(ChannelPtr ChannelPtr);

    void loop();
    void quit();

private:
    std::unique_ptr<Epoll> epollPtr;
    bool quited;
    bool eventHandling;
};
}
}