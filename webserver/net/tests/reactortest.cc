/*
 * @Author: coxlong
 * @Date: 2021-04-10 20:23:44
 * @LastEditTime: 2021-04-10 21:06:25
 */
#include <iostream>
#include <sys/timerfd.h>

#include <webserver/net/EventLoop.h>
#include <webserver/net/Channel.h>

webserver::net::EventLoop* gEventLoop;
void timeout() {
    std::cout << "Timeout!" << std::endl;
    gEventLoop->quit();
}

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);

    webserver::net::EventLoop eventLoop;
    gEventLoop = &eventLoop;
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    webserver::net::ChannelPtr channelPtr= std::make_shared<webserver::net::Channel>(&eventLoop, timerfd);
    channelPtr->setReadCallback(timeout);
    channelPtr->enableReading();
    
    itimerspec howlong;
    
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 1;
    ::timerfd_settime(timerfd, 0, &howlong, nullptr);
    eventLoop.loop();
    
    return 0;
}