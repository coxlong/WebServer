/*
 * @Author: coxlong
 * @Date: 2021-06-12 11:14:39
 * @LastEditTime: 2021-06-12 17:42:29
 */
#pragma once
#include <cstring>
#include <iostream>
#include <glog/logging.h>

namespace webserver {
class MemPool {
public:
    explicit MemPool(int _n=1000) : __N_ONCE(_n) {
        for(auto& fl:free_list) {
            fl = nullptr;
        }
    }

    void* allocate(size_t n) {
        if(n>__MAX_BYTES) {
            // LOG(ERROR) << "allocate, n=" << n;
            return malloc(n);
        } else if(n>0) {
            auto fl_index = FREELIST_INDEX(n);
            if(free_list[fl_index] == nullptr) {
                chunk_alloc(ROUND_UP(n), __N_ONCE);
            }
            auto tmp = free_list[fl_index];
            free_list[fl_index] = free_list[fl_index]->next;
            return (void*)tmp;
        } else {
            return nullptr;
        }
    }

    void deallocate(void* ptr, size_t n) {
        if(n>__MAX_BYTES) {
            // LOG(ERROR) << "deallocate, n=" << n;
            free(ptr);
        } else if(n>0) {
            auto fl_index = FREELIST_INDEX(n);
            ((obj*)ptr)->next = free_list[fl_index];
            free_list[fl_index] = ((obj*)ptr);
        }
    }

private:
    union obj {
        obj* next;
        char client_data[1];
    };
    enum {__ALIGN = 64};
    enum {__MAX_BYTES = 8192};
    enum {__FREE_LIST_LEN = 128};

private:
    size_t ROUND_UP(size_t bytes) {
        return ((bytes) + __ALIGN-1) & ~(__ALIGN-1);
    }
    size_t FREELIST_INDEX(size_t bytes) {
        return ((bytes) + __ALIGN-1)/__ALIGN - 1;
    }

    void chunk_alloc(size_t size, int nobjs) {
        // LOG(ERROR) << "chunk_alloc: " << nobjs;
        char* mem = (char*)malloc(size * nobjs);
        if(mem == 0) {
            std::cerr << "out of memory" << std::endl;
            exit(-1);
        }
        auto& head = free_list[FREELIST_INDEX(size)];
        head = (obj*)mem;
        auto cur = head;
        for(int i=1; i<nobjs; ++i) {
            mem += size;
            cur->next = (obj*)(mem);
            cur = cur->next;
        }
        cur->next = nullptr;
    }
private:
    obj* volatile free_list[__FREE_LIST_LEN];
    int __N_ONCE;
};
}