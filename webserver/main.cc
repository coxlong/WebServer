/*
 * @Author: coxlong
 * @Date: 2021-04-15 22:04:49
 * @LastEditTime: 2021-04-16 19:54:10
 */
#include <webserver/net/TCPServer.h>
#include <webserver/net/EventLoop.h>
#include <glog/logging.h>

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::GLOG_INFO, "./Debug/logtestInfo");
    google::SetLogDestination(google::GLOG_ERROR, "./Debug/logtestError");
    google::SetLogDestination(google::GLOG_FATAL, "./Debug/logtestFatal");

    webserver::net::EventLoop eventLoop;
    
    webserver::net::TCPServer server(&eventLoop, 20);

    server.start();
    LOG(ERROR) << "server start";

    eventLoop.loop();
    
    return 0;
}