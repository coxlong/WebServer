/*
 * @Author: coxlong
 * @Date: 2021-04-11 17:18:28
 * @LastEditTime: 2021-04-15 22:05:06
 */
#include <webserver/net/TCPServer.h>
#include <webserver/net/EventLoop.h>
#include <glog/logging.h>

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);

    webserver::net::EventLoop eventLoop;
    
    webserver::net::TCPServer server(&eventLoop, 3);

    server.start();
    LOG(ERROR) << "server start";

    eventLoop.loop();
    
    return 0;
}