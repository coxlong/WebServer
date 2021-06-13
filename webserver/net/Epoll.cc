/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:10:30
 * @LastEditTime: 2021-06-13 10:30:57
 */
#include <webserver/net/Epoll.h>
#include <webserver/net/Channel.h>

using namespace webserver;
using namespace webserver::net;

const int MaxReadyEventsNum = 4096;
const int EpollWaitTime = 10000;

Epoll::Epoll()
    : epollFd(epoll_create1(EPOLL_CLOEXEC)),
      readyEvents(MaxReadyEventsNum) {
    assert(epollFd > 0);
}

Epoll::~Epoll() {
}

void Epoll::poll(std::vector<ChannelPtr, Alloc<ChannelPtr>>& readyChannelPtr) {
    while(true) {
        auto readyEventsNum = epoll_wait(
            epollFd,
            readyEvents.data(),
            readyEvents.size(),
            EpollWaitTime
        );
        if(readyEventsNum > 0) {
            decltype(channelPtrs.begin()) it;
            for(int i=0; i<readyEventsNum; ++i) {
                auto fd = readyEvents[i].data.fd;
                if((it=channelPtrs.find(fd)) != channelPtrs.end()) {
                    it->second->setRevents(readyEvents[i].events);
                    readyChannelPtr.emplace_back(it->second);
                } else {
                    LOG(ERROR) << "channel(fd=" << fd << ") not exists";
                }
            }
            return;
        } else if(readyEventsNum == 0) {            
            LOG(INFO) << "epoll_wait timeout!";
        } else {
            break;
        }
    }
}

void Epoll::updateChannel(ChannelPtr channelPtr) {
    auto fd = channelPtr->getFd();
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
        }
    } else {
        // fd已存在，修改fd上的注册事件
        if(epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event) < 0) {
            LOG(ERROR) << "epoll_mod error!";
        }
    }
}

void Epoll::removeChannel(const int fd) {
    if(channelPtrs.find(fd) != channelPtrs.end()) {
        if(epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr) < 0) {
            LOG(ERROR) << "epoll_del error! errno=" << errno << " tid=" << CurrentThread::tid();
        }
        channelPtrs.erase(fd);
    } else {
        LOG(ERROR) << "channel not exists";
    }
}

void Epoll::clear() {
    channelPtrs.clear();
    readyEvents.clear();
}