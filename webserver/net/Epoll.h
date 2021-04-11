/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:10:22
 * @LastEditTime: 2021-04-11 21:47:39
 */
#pragma once
#include <unordered_map>
#include <vector>
#include <sys/epoll.h>

#include <webserver/utils/NonCopyable.h>
#include <webserver/net/EventLoop.h>

namespace webserver {
namespace net {

class Epoll : NonCopyable {
public:
    Epoll();
    ~Epoll();

    std::vector<ChannelPtr> poll();
    void updateChannel(ChannelPtr channelPtr);
    void removeChannel(const int fd);

    int getChannelSize() {
        return channelPtrs.size();
    }

private:
    const int epollFd;
    std::unordered_map<int, ChannelPtr> channelPtrs;
    std::vector<epoll_event> readyEvents;
};
}
}