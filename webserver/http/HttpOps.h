/*
 * @Author: coxlong
 * @Date: 2021-04-13 18:32:10
 * @LastEditTime: 2021-04-13 20:44:51
 */
#pragma once
#include <memory>
#include <functional>


namespace webserver {

namespace net {
class EventLoop;
class Channel;
}
using ChannelWeakPtr=std::weak_ptr<net::Channel>;
// using ConnCallback=std::function<void(ChannelWeakPtr)>;
namespace http {
void handleRead(ChannelWeakPtr channelPtr);
}
}