/*
 * @Author: coxlong
 * @Date: 2021-06-12 11:36:43
 * @LastEditTime: 2021-06-12 12:09:32
 */
#include <iostream>
#include <webserver/utils/Allocator.h>
// #include <webserver/utils/MemPool.h>

int main() {
    std::cout << sizeof(webserver::MemPool) << std::endl;
    return 0;
}