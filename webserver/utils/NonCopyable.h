/*
 * @Author: coxlong
 * @Date: 2021-04-10 10:11:45
 * @LastEditTime: 2021-04-10 10:12:46
 */
#pragma once

namespace webserver {
class NonCopyable {
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};
}