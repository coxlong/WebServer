add_subdirectory(utils)
add_subdirectory(http)
add_subdirectory(net)


add_executable(main main.cc)
target_link_libraries(main glog)
target_link_libraries(main webserver_net)
target_link_libraries(main webserver_utils)
target_link_libraries(main webserver_http)
target_link_libraries(main pthread)