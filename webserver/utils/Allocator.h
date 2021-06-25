/*
 * @Author: coxlong
 * @Date: 2021-06-12 11:32:20
 * @LastEditTime: 2021-06-14 10:44:52
 */
#pragma once
#include <climits>
#include <memory>

#include <webserver/utils/MemPool.h>

namespace webserver {


template <class T>
class allocator {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

    template <class U>
    struct rebind {
        typedef allocator<U> other;
    };

    allocator() {
    }

    allocator(const allocator& other) {

    }
    template <typename U>
    allocator(const allocator<U>& other) {}

public:
    pointer allocate(size_type n, const void* hint=0) {
        return (pointer)t_memPoolPtr->allocate(n*sizeof(T)); 
    }

    void deallocate(pointer p, size_type n) {
        t_memPoolPtr->deallocate((void*)p, n*sizeof(T));
    }

    void construct(pointer p, const_reference value) {
        new(p) T(value);
    }

    void destroy(pointer p) {
        p->~T();
    }

    pointer address(reference x) {
        return (pointer)&x;
    }

    size_type max_size() const {
        return size_type(UINT_MAX/sizeof(T));
    }
};

template<class T>
// using Alloc = std::allocator<T>;
using Alloc = allocator<T>;

template<typename _Tp, typename... _Args>
inline std::shared_ptr<_Tp>
make_shared(_Args&&... __args)
{
    typedef typename std::remove_const<_Tp>::type _Tp_nc;
    return std::allocate_shared<_Tp>(Alloc<_Tp_nc>(),
                    std::forward<_Args>(__args)...);
}

typedef std::basic_string<char, std::char_traits<char>, Alloc<char>> string;

}