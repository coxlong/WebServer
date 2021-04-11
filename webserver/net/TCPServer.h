/*
 * @Author: coxlong
 * @Date: 2021-04-11 10:45:25
 * @LastEditTime: 2021-04-11 21:39:19
 */
#pragma once
#include <memory>

#include <webserver/utils/NonCopyable.h>

namespace webserver {
namespace net {

class EventLoop;
class Channel;
using ChannelPtr=std::shared_ptr<Channel>;

class TCPServer : NonCopyable {
public:
    explicit TCPServer(EventLoop* eventLoop);
    ~TCPServer();

    void start();

    void newConnection();
    void handleRead(int connFd);

private:
    const int listenFd;
    EventLoop* eventLoop;
    ChannelPtr channelPtr;
};

}
}