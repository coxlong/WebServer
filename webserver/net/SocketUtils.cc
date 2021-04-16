/*
 * @Author: coxlong
 * @Date: 2021-04-11 16:02:00
 * @LastEditTime: 2021-04-16 21:43:20
 */
#include <fcntl.h>
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
    auto flags = fcntl(sockFd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(sockFd, F_SETFL, flags);
    return sockFd;
}

int listenSocket(int sockfd) {
    return listen(sockfd, 100);
}

int acceptConn(int sockfd) {
    return accept(sockfd, nullptr, 0);
}

int closeConn(int sockfd) {
    return close(sockfd);
}

ssize_t sendMsg(int connFd, std::string&& msg) {
    size_t total = msg.size();
    size_t len=0, ret;

    while(len < total) {
        ret = write(connFd, &msg[len], total-len);
        if(ret < 0) {
            LOG(ERROR) << strerror(errno);
            return -1;
        }
        len += ret;
    }
    return len;
}

ssize_t recvMsg(int connFd, std::string& msg) {
    ssize_t len;
    std::stringstream ss;
    std::string buf(4096, '\0');
    while((len=recv(connFd, &buf[0], 4096, MSG_DONTWAIT))>0) {
        // LOG(ERROR) << "len=" << len;
        if(len > 0) {
            ss.write(buf.data(), len);
        } else if(len < 0) {
            if((errno==EAGAIN) || (errno==EWOULDBLOCK)) {
                break;
            } else {
                LOG(ERROR) << "errno=" << errno << strerror(errno);
                return 0;
            }
        } else {
            return 0;
        }
    }    
    msg = ss.str();
    return msg.size();
}

}
}