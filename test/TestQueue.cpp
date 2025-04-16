#include "JobQueue.hpp"
#include <iostream>

int main() {
    JobQueue& jobQueue = JobQueue::getInstance();

    // 添加一些任务到队列中
    JobInfo job1{.status = JobStatus::Queuing, .job_id = "1"};
    JobInfo job2{.status = JobStatus::Suspending, .job_id = "2"};
    JobInfo job3{.status = JobStatus::Retry, .job_id = "3"};

    jobQueue.enqueue(JobManager(job1));
    jobQueue.enqueue(JobManager(job2));
    jobQueue.enqueue(JobManager(job3));

    std::cout << "size: " << jobQueue.size() << std::endl;

    auto res = jobQueue.dequeue();
    if (!res.has_value())
    {
        std::cout << "Dequeued job with ID: " << "空" << std::endl;
    }
    auto dequeuedJob = res.value();
    std::cout << "Dequeued job with ID: " << dequeuedJob.getJobId().value() << std::endl;

    res = jobQueue.dequeue();
    if (!res.has_value())
    {
        std::cout << "Dequeued job with ID: " << "空" << std::endl;
    }

    res = jobQueue.dequeue();
    if (!res.has_value())
    {
        std::cout << "Dequeued job with ID: " << "空" << std::endl;
    }
    dequeuedJob = res.value();
    std::cout << "Dequeued job with ID: " << dequeuedJob.getJobId().value() << std::endl;

    std::cout << "size: " << jobQueue.size() << std::endl;

    jobQueue.enqueue(JobManager(job1));
    jobQueue.enqueue(JobManager(job3));

    // 根据 job_id 移除任务
    auto removedJob = jobQueue.dequeueByJobId("2");
    if (removedJob) {
        std::cout << "Removed job with ID: " << removedJob->getJobId().value() << std::endl;
    } else {
        std::cout << "Job with ID 2 not found!" << std::endl;
    }

    std::cout << "size: " << jobQueue.size() << std::endl;

    return 0;
}