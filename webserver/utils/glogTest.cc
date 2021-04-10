/*
 * @Author: coxlong
 * @Date: 2021-04-10 09:52:55
 * @LastEditTime: 2021-04-10 10:07:50
 */
#include <glog/logging.h>

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);

    // ...
    LOG(INFO) << "Found " ;
}