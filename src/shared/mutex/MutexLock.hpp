#pragma once

#include "NonCopyable.hpp"
#include <pthread.h>
#include <cerrno> // 包含此头文件以访问 errno 及相关错误码

/**
 * @brief 互斥锁类，提供基本的线程同步机制。
 *
 * MutexLock 类封装了 pthread_mutex_t，并提供了加锁、尝试加锁和解锁的方法。
 * 它继承自 NonCopyable，确保互斥锁对象不能被复制，从而避免潜在的资源管理问题。
 *
 * 使用 MutexLockGuard 类来自动管理锁定和解锁操作，以确保即使在发生异常的情况下也能正确释放锁。
 */
class MutexLock
: NonCopyable
{
public:
    /**
     * @brief 构造函数，初始化互斥锁。
     *
     * 调用 pthread_mutex_init 初始化 mutex_ 成员变量。
     */
    MutexLock();

    /**
     * @brief 析构函数，销毁互斥锁。
     *
     * 调用 pthread_mutex_destroy 销毁 mutex_ 成员变量。
     */
    ~MutexLock();

    /**
     * @brief 加锁，阻塞当前线程直到获得锁。
     *
     * 调用 pthread_mutex_lock 来获取锁。
     */
    void lock();

    /**
     * @brief 尝试加锁，不阻塞当前线程。
     *
     * 如果成功获取锁，则返回 true；否则立即返回 false。
     *
     * @return bool 是否成功获取锁。
     */
    bool tryLock();

    /**
     * @brief 解锁，释放当前持有的锁。
     *
     * 调用 pthread_mutex_unlock 来释放锁。
     */
    void unlock();

    /**
     * @brief 获取内部的 pthread_mutex_t 指针。
     *
     * 主要用于与需要原始 pthread_mutex_t 的 API 进行交互。
     *
     * @return pthread_mutex_t* 内部的互斥锁指针。
     */
    pthread_mutex_t *getMutexLockPtr();

private:
    pthread_mutex_t mutex_; ///< 内部使用的 POSIX 线程互斥锁。
};

/**
 * @brief 自动管理互斥锁的 RAII 风格的锁管理器。
 *
 * MutexLockGuard 类通过其构造函数和析构函数自动管理 MutexLock 对象的加锁和解锁操作。
 * 当 MutexLockGuard 对象创建时，它会自动调用 MutexLock::lock() 方法加锁；
 * 当 MutexLockGuard 对象在其作用域结束时被销毁（即离开当前代码块），它会自动调用 MutexLock::unlock() 方法解锁。
 * 这种 RAII（Resource Acquisition Is Initialization）风格的设计可以有效避免死锁和资源泄漏，
 * 并且即使在发生异常的情况下也能确保锁被正确释放。
 */
class MutexLockGuard
{
public:
    /**
     * @brief 构造函数，自动加锁。
     *
     * 在构造时自动调用 MutexLock::lock() 方法对传入的 mutex 进行加锁。
     *
     * @param mutex 需要管理的 MutexLock 对象引用。
     */
    explicit MutexLockGuard(MutexLock &mutex)
        : mutex_(mutex) {
        mutex_.lock();
    }

    /**
     * @brief 析构函数，自动解锁。
     *
     * 在析构时自动调用 MutexLock::unlock() 方法对传入的 mutex 进行解锁。
     */
    ~MutexLockGuard() {
        mutex_.unlock();
    }

private:
    MutexLock &mutex_;
};