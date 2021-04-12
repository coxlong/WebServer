set(webserver_utils
    Thread.cc
)

add_library(webserver_utils ${webserver_utils})

add_subdirectory(tests)