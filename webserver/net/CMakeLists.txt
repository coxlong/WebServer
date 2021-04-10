set(webserver_net
    EventLoop.cc
    Epoll.cc
    Channel.cc
)

add_library(webserver_net ${webserver_net})

add_subdirectory(tests)