/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:10:22
 * @LastEditTime: 2021-06-12 21:37:25
 */
#pragma once
#include <unordered_map>
#include <vector>
#include <sys/epoll.h>

#include <webserver/utils/NonCopyable.h>
#include <webserver/net/EventLoop.h>
#include <webserver/utils/Allocator.h>

namespace webserver {
namespace net {

class Epoll : NonCopyable {
public:
    Epoll();
    ~Epoll();

    void poll(std::vector<ChannelPtr, Alloc<ChannelPtr>>& readyChannelPtrs);
    void updateChannel(ChannelPtr channelPtr);
    void removeChannel(const int fd);

    void clear();

    int getChannelSize() {
        return channelPtrs.size();
    }

private:
    const int epollFd;
    std::unordered_map<int, ChannelPtr, std::hash<int>, std::equal_to<int>, Alloc< std::pair<const int, ChannelPtr> > > channelPtrs;
    std::vector<epoll_event, Alloc<epoll_event>> readyEvents;
};
}
}