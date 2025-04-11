#include "JobManager.hpp"

JobManager::JobManager(const JobInfo& job_info) {
    job_info_ = std::make_unique<JobInfo>(job_info);
}

JobManager::JobManager(const JobManager& other) {
    if (other.job_info_) {
        job_info_ = std::make_unique<JobInfo>(*other.job_info_);
    }
}

JobManager::JobManager(JobManager&& other) noexcept {
    job_info_ = std::move(other.job_info_);
}

JobManager& JobManager::operator=(const JobManager& other) {
    if (this != &other) {
        if (other.job_info_) {
            job_info_ = std::make_unique<JobInfo>(*other.job_info_);
        } else {
            job_info_.reset();
        }
    }
    return *this;
}

JobManager& JobManager::operator=(JobManager&& other) noexcept {
    if (this != &other) {
        job_info_ = std::move(other.job_info_);
    }
    return *this;
}

std::optional<JobInfo> JobManager::findJobByJobId(const std::string& job_id) const {
    if (job_info_ && job_info_->job_id == job_id) {
        return *job_info_;
    }
    return std::nullopt;
}

std::optional<JobInfo> JobManager::getJobInfo() const {
    if (job_info_) {
        return *job_info_;
    }
    return std::nullopt;
}