#include "JobQueue.hpp"

/**
 * @brief 获取 JobQueue 的唯一实例。
 *
 * 使用静态局部变量实现懒汉式单例模式，确保线程安全（C++11 及以上标准支持）。
 *
 * @return JobQueue& 返回 JobQueue 的引用。
 */
JobQueue& JobQueue::getInstance() {
    static JobQueue instance; // 静态局部变量，确保唯一性和线程安全
    return instance;
}

/**
 * @brief 自定义出队逻辑，支持根据任务状态决定是否放回队尾。
 *
 * 如果当前任务状态为 Suspending 或 Cancelling，则将其放回队尾并继续尝试出队。
 * 如果状态为 Queuing、Retry 或 Resume，则正常返回任务。
 *
 * @return JobManager 返回符合条件的任务。
 */
JobManager JobQueue::dequeue() {
    while (!empty()) {
        // 调用基类的 pop_front 方法取出队首任务
        JobManager job = pop_front();

        // 检查任务状态
        auto jobInfo = job.getJobInfo();
        if (jobInfo && 
            (jobInfo->status == JobStatus::Suspending ||
             jobInfo->status == JobStatus::Cancelled)) {
            // 如果状态为 Suspending 或 Cancelling，放回队尾
            push_back(job);
        } else if (jobInfo && 
                   (jobInfo->status == JobStatus::Queuing ||
                    jobInfo->status == JobStatus::Retry ||
                    jobInfo->status == JobStatus::Resume)) {
            // 如果状态为 Queuing、Retry 或 Resume，则正常返回任务
            return job;
        }
    }

    // 如果队列为空，抛出异常
    throw std::runtime_error("No valid jobs available in the queue.");
}

/**
 * @brief 封装基类的 push_back 方法，提供统一的入队接口。
 *
 * @param job 要入队的任务。
 */
void JobQueue::enqueue(const JobManager& job) {
    push_back(job);
}