/*
 * @Author: coxlong
 * @Date: 2021-04-11 16:01:54
 * @LastEditTime: 2021-04-16 21:43:30
 */
#pragma once
#include <string>

namespace webserver {
namespace net {

int initSocket();
int listenSocket(int sockfd);
int acceptConn(int sockfd);
int closeConn(int sockfd);
ssize_t sendMsg(int connFd, std::string&& msg);
ssize_t recvMsg(int connFd, std::string& buf);

}
}