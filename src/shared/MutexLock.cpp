#include "MutexLock.hpp"
#include <stdio.h>

MutexLock::MutexLock()
{
    int ret = pthread_mutex_init(&mutex_, nullptr);
    if (ret != 0) {
        perror("pthread_mutex_init");
        // 可以考虑抛出异常或其他错误处理方式
    }
}

MutexLock::~MutexLock()
{
    int ret = pthread_mutex_destroy(&mutex_);
    if (ret != 0) {
        perror("pthread_mutex_destroy");
        // 可以考虑抛出异常或其他错误处理方式
    }
}

void MutexLock::lock()
{
    int ret = pthread_mutex_lock(&mutex_);
    if (ret != 0) {
        perror("pthread_mutex_lock");
        // 可以考虑抛出异常或其他错误处理方式
    }
}

bool MutexLock::tryLock()
{
    int ret = pthread_mutex_trylock(&mutex_);
    if (ret == EBUSY) {
        return false;
    } else if (ret != 0) {
        perror("pthread_mutex_trylock");
        // 可以考虑抛出异常或其他错误处理方式
    }
    return true;
}

void MutexLock::unlock()
{
    int ret = pthread_mutex_unlock(&mutex_);
    if (ret != 0) {
        perror("pthread_mutex_unlock");
        // 可以考虑抛出异常或其他错误处理方式
    }
}

pthread_mutex_t* MutexLock::getMutexLockPtr()
{
    return &mutex_;
}