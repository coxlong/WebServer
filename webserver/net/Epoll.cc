/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:10:30
 * @LastEditTime: 2021-04-11 21:48:01
 */
#include <webserver/net/Epoll.h>
#include <webserver/net/Channel.h>

using namespace webserver;
using namespace webserver::net;

const int MaxReadyEventsNum = 4096;
const int EpollWaitTime = 1;

Epoll::Epoll()
    : epollFd(epoll_create1(EPOLL_CLOEXEC)),
      readyEvents(MaxReadyEventsNum) {
    assert(epollFd > 0);
}

Epoll::~Epoll() {}

std::vector<ChannelPtr> Epoll::poll() {
    while(true) {
        // LOG(ERROR) << "epoll_wait begin";
        auto readyEventsNum = epoll_wait(
            epollFd,
            readyEvents.data(),
            readyEvents.size(),
            EpollWaitTime
        );
        // LOG(ERROR) << "epoll_wait end";
        // LOG(ERROR) << "readyEventsNum=" << readyEventsNum;
        if(readyEventsNum < 0) {
            LOG(ERROR) << "epoll_wait timeout!";
        } else {
            std::vector<ChannelPtr> res;
            decltype(channelPtrs.begin()) it;
            for(int i=0; i<readyEventsNum; ++i) {
                auto fd = readyEvents[i].data.fd;
                if((it=channelPtrs.find(fd)) != channelPtrs.end()) {
                    it->second->setRevents(readyEvents[i].events);
                    res.emplace_back(it->second);
                } else {
                    LOG(ERROR) << "channelPtr(fd=" << fd << ") not exists";
                }
            }
            return res;
        }
    }
    return std::vector<ChannelPtr>();
}

void Epoll::updateChannel(ChannelPtr channelPtr) {
    auto fd = channelPtr->getFd();
    LOG(ERROR) << "updateChannel: fd=" << fd;
    epoll_event event;
    event.data.fd = fd;
    event.events = channelPtr->getEvents();
    if(channelPtrs.find(fd) == channelPtrs.end()) {
        // 往事件表上注册fd上的事件
        if(epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) < 0) {
            // 操作失败
            LOG(ERROR) << "epoll_add error!";
        } else {
            // 将channelPtr添加到channelPtrs
            channelPtrs[fd] = channelPtr;
            LOG(INFO) << "epoll_add(fd=" << fd << ") successful!";
        }
    } else {
        // fd已存在，修改fd上的注册事件
        if(epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event) < 0) {
            LOG(ERROR) << "epoll_mod error!";
        }
    }
}

void Epoll::removeChannel(const int fd) {
    // LOG(ERROR) << "---";
    // LOG(ERROR) << "removeChannel: fd=" << fd;
    // LOG(ERROR) << "channel num=" << channelPtrs.size();
    // for(auto c:channelPtrs) {
    //     LOG(ERROR) << "channelFd:" << c.second->getFd();
    // }
    // LOG(ERROR) << "---";
    if(channelPtrs.find(fd) != channelPtrs.end()) {
        if(epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr) < 0) {
            LOG(ERROR) << "epoll_del error!";
        }
        // LOG(ERROR) << "count=" << channelPtrs.find(fd)->second.use_count();
        channelPtrs.erase(fd);
        // ::close(fd);
        LOG(ERROR) << "channel delete successful";
    } else {
        LOG(ERROR) << "channel not exist";
    }
}