#pragma once

#include <string>
#include <optional>
#include <memory>

/**
 * @brief 任务状态枚举类型。
 *
 * 使用强类型枚举（enum class）定义任务状态，避免魔法数字带来的可读性问题。
 */
enum class JobStatus : int {
    Starting = 1,   ///< 启动中
    Indexing = 2,   ///< 索引中
    Queuing = 3,    ///< 排队中
    Running = 4,    ///< 运行中
    Suspending = 5, ///< 暂停中
    Succeed = 6,    ///< 成功完成
    Failed = 7,     ///< 失败
    Cancelled = 8,  ///< 被用户取消
    Retry = 9,      ///< 重试中
    Resume = 10     ///< 恢复运行
};

/**
 * @brief JobInfo 结构体，用于存储任务的详细信息。
 */
struct JobInfo {
    JobStatus status;                   // 任务状态：0: 预, 1: Starting, 2: Indexing, 3: queuing, 4: Running, 5: suspended
    std::string job_id;           // 任务 ID
};

/**
 * @brief JobManager 类，用于管理和操作 JobInfo 对象。
 *
 * JobManager 封装了对 JobInfo 的管理功能，提供了任务信息的创建、拷贝、移动、查找和更新操作。
 * 它通过智能指针（std::unique_ptr）管理资源，确保任务信息的安全性和一致性，并支持基于唯一标识符（job_id）
 * 的高效任务查找。
 */
class JobManager {
private:
    std::unique_ptr<JobInfo> job_info_; ///< 存储任务信息的智能指针，确保资源安全和自动释放。

public:
    /**
     * @brief 构造函数，初始化 JobManager 对象。
     *
     * 使用传入的 JobInfo 对象初始化当前管理的任务信息。
     *
     * @param job_info 包含任务详细信息的 JobInfo 对象。
     */
    explicit JobManager(const JobInfo& job_info);

    /**
     * @brief 拷贝构造函数，深拷贝任务信息。
     *
     * 创建一个新的 JobManager 对象，并深拷贝另一个对象中存储的任务信息。
     * 确保新对象与原对象互不影响。
     *
     * @param other 另一个 JobManager 对象。
     */
    JobManager(const JobManager& other);

    /**
     * @brief 移动构造函数，转移任务信息的所有权。
     *
     * 创建一个新的 JobManager 对象，并通过移动语义高效地转移任务信息的所有权。
     *
     * @param other 另一个 JobManager 对象。
     */
    JobManager(JobManager&& other) noexcept;

    /**
     * @brief 拷贝赋值操作符，深拷贝任务信息。
     *
     * 将另一个 JobManager 对象中的任务信息深拷贝到当前对象中。
     * 如果当前对象已经管理了一个任务信息，则会释放原有资源。
     *
     * @param other 另一个 JobManager 对象。
     * @return 返回当前对象的引用。
     */
    JobManager& operator=(const JobManager& other);

    /**
     * @brief 移动赋值操作符，转移任务信息的所有权。
     *
     * 将另一个 JobManager 对象中的任务信息通过移动语义高效地转移到当前对象中。
     * 如果当前对象已经管理了一个任务信息，则会释放原有资源。
     *
     * @param other 另一个 JobManager 对象。
     * @return 返回当前对象的引用。
     */
    JobManager& operator=(JobManager&& other) noexcept;

    /**
     * @brief 根据任务 ID 查找任务信息。
     *
     * 在当前管理的任务信息中查找指定 ID 的任务。如果找到匹配的任务信息，
     * 则返回包含该任务信息的 std::optional 对象；否则返回 std::nullopt。
     *
     * @param job_id 要查找的任务 ID。
     * @return 包含任务信息的 std::optional 对象。如果未找到任务，则返回 std::nullopt。
     */
    std::optional<JobInfo> findJobByJobId(const std::string& job_id) const;

    /**
     * @brief 返回任务的 JobInfo。
     *
     * 如果任务信息存在，则返回包含 JobInfo 的 std::optional；否则返回 std::nullopt。
     *
     * @return std::optional<JobInfo> 包含任务信息的可选对象。
     */
    std::optional<JobInfo> getJobInfo() const;

    /**
     * @brief 返回任务的 JobInfo。
     *
     * 如果任务信息存在，则返回包含 JobInfo 的 std::optional；否则返回 std::nullopt。
     *
     * @return std::optional<JobInfo> 包含任务信息的可选对象。
     */
    std::optional<std::string> getJobId() const;
};