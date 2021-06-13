/*
 * @Author: coxlong
 * @Date: 2021-04-11 10:45:25
 * @LastEditTime: 2021-06-13 10:19:31
 */
#pragma once
#include <memory>
#include <functional>

#include <webserver/utils/NonCopyable.h>

namespace webserver {
namespace net {

class EventLoop;
class Channel;
class EventLoopThreadPool;
using ChannelPtr=std::shared_ptr<Channel>;
using ChannelWeakPtr=std::weak_ptr<Channel>;
using ConnCallback=std::function<void(ChannelWeakPtr)>;

class TCPServer : NonCopyable {
public:
    explicit TCPServer(std::shared_ptr<EventLoop> eventLoopPtr, const int threadNum=5);
    ~TCPServer();

    void start();
    void stop();
    void newConnection();

private:
    const int listenFd;
    std::shared_ptr<EventLoop> eventLoopPtr;
    ChannelPtr channelPtr;
    const int threadNum;
    ConnCallback connReadCallback;
    std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool;
};
}
}