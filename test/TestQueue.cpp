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

    // 自定义出队逻辑
    try {
        while (!jobQueue.empty()) {
            JobManager dequeuedJob = jobQueue.dequeue();
            std::cout << "Dequeued job with ID: " << dequeuedJob.findJobByJobId(dequeuedJob.getJobInfo()->job_id)->job_id << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}