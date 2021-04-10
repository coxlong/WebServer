/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:09:59
 * @LastEditTime: 2021-04-10 19:20:58
 */
#include <webserver/net/EventLoop.h>
#include <webserver/net/Epoll.h>
#include <webserver/net/Channel.h>

using namespace webserver;
using namespace webserver::net;

EventLoop::EventLoop()
    : epollPtr(std::make_unique<Epoll>()),
      quited(false),
      eventHandling(false) {
}

EventLoop::~EventLoop() {}

void EventLoop::updateChannel(ChannelPtr channelPtr) {
    epollPtr->updateChannel(channelPtr);
}

void EventLoop::removeChannel(ChannelPtr channelPtr) {
    epollPtr->removeChannel(channelPtr);
}

void EventLoop::loop() {
    while(!quited) {
        auto readyChannles = epollPtr->poll();
        eventHandling = true;
        for(auto channel:readyChannles) {
            channel->handleEvents();
        }
        eventHandling = false;
    }
}

void EventLoop::quit() {
    quited = true;
}