#pragma once

#include "shared/queue/BaseQueue.hpp"
#include "TapeDrivesOperation.hpp"

/**
 * @brief 单例模式下的磁带驱动器操作队列。
 *
 * TapeDrivesQueue 类继承自 BaseQueue<TapeDrivesOperation>，专门用于管理和调度磁带驱动器相关的操作。
 * 该类实现了单例模式，确保在整个应用程序中只有一个 TapeDrivesQueue 实例存在。
 * 同时，通过删除拷贝构造函数和赋值操作符，防止对象被复制或赋值，从而避免潜在的资源管理问题。
 */
class TapeDrivesQueue : public BaseQueue<TapeDrivesOperation> {
public:
    /**
     * @brief 获取 TapeDrivesQueue 的唯一实例。
     *
     * 使用单例模式确保整个应用程序中只有一个 TapeDrivesQueue 实例存在。
     *
     * @return TapeDrivesQueue& 返回 TapeDrivesQueue 的引用。
     */
    static TapeDrivesQueue& getInstance();

private:
    /**
     * @brief 默认构造函数（私有）。
     *
     * 将构造函数设为私有，防止外部代码直接实例化 TapeDrivesQueue 对象。
     */
    TapeDrivesQueue() = default;

    /**
     * @brief 拷贝构造函数（已删除）。
     *
     * 禁止拷贝构造，防止对象被复制。
     */
    TapeDrivesQueue(const TapeDrivesQueue&) = delete;

    /**
     * @brief 赋值操作符（已删除）。
     *
     * 禁止赋值操作，防止对象被赋值。
     *
     * @return TapeDrivesQueue& 返回引用自身（未定义行为，因为该函数已被删除）。
     */
    TapeDrivesQueue& operator=(const TapeDrivesQueue&) = delete;
};