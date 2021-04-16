/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:10:07
 * @LastEditTime: 2021-04-16 21:13:37
 */
#pragma once
#include <functional>
#include <memory>

#include <webserver/utils/NonCopyable.h>

namespace webserver {
namespace net {

class EventLoop;

class Channel : NonCopyable, public std::enable_shared_from_this<Channel> {
public:
    using EventCallback=std::function<void()>;

    Channel(EventLoop* ownerLoop, const int fd);
    ~Channel();

    void handleEvents();
    void enableReading();
    void enableWriting();

public:
    int getFd() const {
        return fd;
    }
    __uint32_t getEvents() const {
        return events;
    }
    void setEvents(const __uint32_t events) {
        this->events = events;
    }
    void setRevents(const __uint32_t revents) {
        this->revents = revents;
    }

    void setEpollET();

    void setReadCallback(const EventCallback callback) {
        readCallback = callback;
    }
    void setWriteCallback(const EventCallback callback) {
        writeCallback = callback;
    }

    EventLoop* getOwnerLoop() const { return ownerLoop; }

private:
    EventLoop* ownerLoop;
    const int fd;
    __uint32_t events;      // 注册事件
    __uint32_t revents;     // 就绪事件

    EventCallback readCallback;
    EventCallback writeCallback;
    EventCallback errorCallback;
};
}
}