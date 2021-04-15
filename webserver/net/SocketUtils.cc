/*
 * @Author: coxlong
 * @Date: 2021-04-11 16:02:00
 * @LastEditTime: 2021-04-11 21:18:38
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>

#include <glog/logging.h>

#include <webserver/net/SocketUtils.h>

namespace webserver {
namespace net {

int initSocket() {
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockFd == -1) {
        return -1;
    }

    sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(18080);
    inet_pton(AF_INET, "", &address.sin_addr);
    
    if(bind(sockFd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1) {
        close(sockFd);
        return -1;
    }
    return sockFd;
}

int listenSocket(int sockfd) {
    return listen(sockfd, 100);
}

int acceptConn(int sockfd, std::string& peerInfo) {
    sockaddr_in client;
    socklen_t len = sizeof(client);
    auto connfd = accept(sockfd, reinterpret_cast<sockaddr*>(&client), &len);
    peerInfo = std::string(inet_ntoa(client.sin_addr)) + "::" + std::to_string(client.sin_port);
    return connfd;
}

int closeConn(int sockfd) {
    return close(sockfd);
}

ssize_t sendMsg(int connFd, std::string msg) {
    auto res = write(connFd, msg.c_str(), msg.length());
    if(res==-1) {
        LOG(ERROR) << "wite error errno=" << errno;
    }
    return res;
}

ssize_t recvMsg(int connFd, std::string& sbuf) {
    char buf[4096];
    
    sbuf.clear();

    ssize_t readN;
    while((readN=read(connFd, buf, 1024))>0) {
        sbuf.append(buf, readN);
        if(readN != 4096) {
            break;
        }
    }

    return sbuf.size();
}

}
}