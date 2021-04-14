/*
 * @Author: coxlong
 * @Date: 2021-04-11 16:01:54
 * @LastEditTime: 2021-04-11 21:21:17
 */
#pragma once
#include <string>

namespace webserver {
namespace net {

int initSocket();
int listenSocket(int sockfd);
int acceptConn(int sockfd, std::string& peerInfo);
int closeConn(int sockfd);
ssize_t sendMsg(int connFd, std::string msg);
ssize_t recvMsg(int connFd, std::string& buf);

}
}