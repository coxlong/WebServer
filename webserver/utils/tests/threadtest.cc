#include <webserver/utils/Thread.h>
#include <iostream>
#include <glog/logging.h>
#include <functional>


void threadFunc(int) {
    std::cout << "threadFunc: ok" << std::endl;
}

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);
    webserver::Thread thread(std::bind(&threadFunc, 1));
    std::cout << thread.getptid() << std::endl;

    thread.start();
    std::cout << thread.getptid() << std::endl;
    thread.join();

    std::cout << "main: ok" << std::endl;

    return 0;
}