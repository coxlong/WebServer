/*
 * @Author: coxlong
 * @Date: 2021-04-11 10:45:25
 * @LastEditTime: 2021-04-12 23:18:47
 */
#pragma once
#include <memory>

#include <webserver/utils/NonCopyable.h>

namespace webserver {
namespace net {

class EventLoop;
class Channel;
class EventLoopThreadPool;
using ChannelPtr=std::shared_ptr<Channel>;

class TCPServer : NonCopyable {
public:
    explicit TCPServer(EventLoop* eventLoop, const int threadNum=5);
    ~TCPServer();

    void start();

    void newConnection();
    void handleRead(int connFd, int loopTid);

private:
    const int listenFd;
    EventLoop* eventLoop;
    ChannelPtr channelPtr;
    const int threadNum;
    std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool;
};

}
}