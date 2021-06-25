/*
 * @Author: coxlong
 * @Date: 2021-06-12 11:14:39
 * @LastEditTime: 2021-06-25 10:32:03
 */
#pragma once
#include <cstring>
#include <iostream>
#include <glog/logging.h>

#include <webserver/utils/Thread.h>

namespace webserver {

class MemPool {
public:
    explicit MemPool(int _n=1000) : __N_ONCE(_n) {
        for(auto& fl:free_list) {
            fl = nullptr;
        }
    }

    ~MemPool() {
        LOG(ERROR) << "~MemPool; memChunk.size=" << memChunk.size();
        for(auto& p:memChunk) {
            free(p);
            p = nullptr;
        }
    }

    void* allocate(size_t n) {
        if(n>__MAX_BYTES) {
            // LOG(ERROR) << "allocate: " << n;
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
    enum {__ALIGN = 8};
    enum {__MAX_BYTES = 128};
    enum {__FREE_LIST_LEN = 16};

private:
    size_t ROUND_UP(size_t bytes) {
        return ((bytes) + __ALIGN-1) & ~(__ALIGN-1);
    }
    size_t FREELIST_INDEX(size_t bytes) {
        return ((bytes) + __ALIGN-1)/__ALIGN - 1;
    }

    void chunk_alloc(size_t size, int nobjs) {
        char* mem = (char*)malloc(size * nobjs);
        if(mem == 0) {
            LOG(FATAL) << "out of memory";
        }
        memChunk.emplace_back((void*)mem);
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
    std::vector<void*> memChunk;
};

extern __thread  MemPool* t_memPoolPtr;

class MemPoolRes {
public:
    MemPoolRes() {
        if(t_memPoolPtr==nullptr) {
            t_memPoolPtr = new MemPool();
        }
    }

    ~MemPoolRes() {
        if(t_memPoolPtr) {
            delete t_memPoolPtr;
            t_memPoolPtr = nullptr;
        }
    }
};

}