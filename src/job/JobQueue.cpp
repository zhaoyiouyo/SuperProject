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
 * @return 包含任务的 std::optional 对象。如果未找到任务，则返回 std::nullopt。
 */
std::optional<JobManager> JobQueue::dequeue() {
    if (!empty()) {
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
            job_map_.erase(job.getJobId().value()); // 从哈希表中移除任务
            return job;
        }
    }
    // 如果队列为空，或不满足出队条件，返回 std::nullopt
    return std::nullopt;
}

/**
 * @brief 封装基类的 push_back 方法，提供统一的入队接口。
 *
 * @param job 要入队的任务。
 */
void JobQueue::enqueue(const JobManager& job) {
    // 调用基类的 push_back 方法
    push_back(job);

    // 获取新插入节点的指针
    Node* newNode = tail_;

    // 将 job_id 和节点指针存入哈希表
    job_map_[job.getJobId().value()] = newNode;
}

std::optional<JobManager> JobQueue::dequeueByJobId(const std::string& job_id) {
    // 在哈希表中查找 job_id
    auto it = job_map_.find(job_id);
    if (it == job_map_.end()) {
        return std::nullopt; // 未找到任务
    }

    // 获取要移除的节点
    Node* nodeToRemove = it->second;

    // 从队列中移除节点
    JobManager job = remove(nodeToRemove);

    // 从哈希表中移除 job_id
    job_map_.erase(it);

    // 返回移除的任务
    return job;
}