#pragma once

#include <unordered_map>

#include "shared/queue/BaseQueue.hpp"
#include "JobManager.hpp"

/**
 * @brief 单例模式下的任务管理队列。
 *
 * JobQueue 类继承自 BaseQueue<JobManager>，专门用于管理和调度任务（JobManager）相关的操作。
 * 该类实现了单例模式，确保在整个应用程序中只有一个 JobQueue 实例存在。
 * 同时，通过删除拷贝构造函数和赋值操作符，防止对象被复制或赋值，从而避免潜在的资源管理问题。
 */
class JobQueue : public BaseQueue<JobManager> {
public:
    /**
     * @brief 获取 JobQueue 的唯一实例。
     *
     * 使用单例模式确保整个应用程序中只有一个 JobQueue 实例存在。
     *
     * @return JobQueue& 返回 JobQueue 的引用。
     */
    static JobQueue& getInstance();

    /**
     * @brief 自定义出队逻辑，支持根据任务状态决定是否放回队尾。
     *
     * 如果当前任务状态为 Suspending 或 Cancelling，则将其放回队尾并继续尝试出队。
     * 如果状态为 Queuing、Retry 或 Resume，则正常返回任务。
     *
     * @return 包含任务的 std::optional 对象。如果未找到任务，则返回 std::nullopt。
     */
    std::optional<JobManager> dequeue();
 
    /**
     * @brief 封装基类的 push_back 方法，提供统一的入队接口。
     *
     * @param job 要入队的任务。
     */
    void enqueue(const JobManager& job);

    /**
     * @brief 根据 job_id 移除指定任务。
     *
     * - 如果找到对应的任务，则从队列中移除并返回该任务。
     * - 如果未找到任务，则返回 std::nullopt。
     *
     * @param job_id 要移除的任务 ID。
     * @return 包含任务的 std::optional 对象。如果未找到任务，则返回 std::nullopt。
     */
    std::optional<JobManager> dequeueByJobId(const std::string& job_id);

private:
    /**
     * @brief 默认构造函数（私有）。
     *
     * 将构造函数设为私有，防止外部代码直接实例化 JobQueue 对象。
     */
    JobQueue() = default;

    /**
     * @brief 拷贝构造函数（已删除）。
     *
     * 禁止拷贝构造，防止对象被复制。
     */
    JobQueue(const JobQueue&) = delete;

    /**
     * @brief 赋值操作符（已删除）。
     *
     * 禁止赋值操作，防止对象被赋值。
     *
     * @return JobQueue& 返回引用自身（未定义行为，因为该函数已被删除）。
     */
    JobQueue& operator=(const JobQueue&) = delete;

    /**
     * @brief 哈希表，用于存储 job_id 和对应节点的映射。
     *
     * - 键：任务 ID（std::string）。
     * - 值：指向任务节点的指针（BaseQueue<T>::Node*）。
     */
    std::unordered_map<std::string, Node*> job_map_;
};