/*
 * @Author: coxlong
 * @Date: 2021-04-15 22:04:49
 * @LastEditTime: 2021-06-13 18:07:07
 */
#include <signal.h>
#include <glog/logging.h>
#include <webserver/net/TCPServer.h>
#include <webserver/net/EventLoop.h>

static void InitSignalHandler() {
    signal(SIGPIPE, SIG_IGN);
}

std::shared_ptr<webserver::net::TCPServer> g_serverPtr(nullptr);

static void stop(int signo) {
    g_serverPtr->stop();
}

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);
    // google::SetLogDestination(google::GLOG_INFO, "./Debug/logtestInfo");
    // google::SetLogDestination(google::GLOG_ERROR, "./Debug/logtestError");
    // google::SetLogDestination(google::GLOG_FATAL, "./Debug/logtestFatal");

    InitSignalHandler();
    signal(SIGINT, stop);
    
    auto eventLoopPtr = std::make_shared<webserver::net::EventLoop>();
    eventLoopPtr->init();

    auto serverPtr = std::make_shared<webserver::net::TCPServer>(eventLoopPtr, 10);
    
    g_serverPtr = serverPtr;

    serverPtr->start();
    LOG(ERROR) << "server start";

    eventLoopPtr->loop();
    LOG(ERROR) << "server stop";
    
    return 0;
}