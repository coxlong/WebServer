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

#include <iostream>

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
    address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    
    if(bind(sockFd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1) {
        close(sockFd);
        return -1;
    }
    return sockFd;
}

int listenSocket(int sockfd) {
    return listen(sockfd, 100);
}

int acceptConn(int sockfd) {
    sockaddr_in client;
    socklen_t len = sizeof(client);
    return accept(sockfd, reinterpret_cast<sockaddr*>(&client), &len);
}

void sendMsg(int connFd, std::string msg) {
    write(connFd, msg.c_str(), msg.length());
}

ssize_t recvMsg(int connFd, char *buf, int bufSize) {
    return read(connFd, buf, bufSize);
}

}
}