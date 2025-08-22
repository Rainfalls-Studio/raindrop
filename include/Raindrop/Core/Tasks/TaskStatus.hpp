#pragma once

#include "Raindrop/Core/Time/Clock.hpp"

namespace Raindrop::Tasks{
    struct CompletedStatus{};

    struct RetryStatus{
        Time::Duration waitDuration;
    };

    struct FailedStatus{
        std::string reason;
    };

    struct TaskStatus{
        enum Type {
            COMPLETED,
            RETRY,
            FAILED
        } type = COMPLETED;

        std::variant<
            CompletedStatus,
            RetryStatus,
            FailedStatus
        > status;

        CompletedStatus& getCompleted(){
            return std::get<CompletedStatus>(status);
        }

        RetryStatus& getRetry(){
            return std::get<RetryStatus>(status);
        }

        FailedStatus& getFailed(){
            return std::get<FailedStatus>(status);
        }

        static inline constexpr TaskStatus Completed(){
            return TaskStatus{
                TaskStatus::COMPLETED,
                CompletedStatus{}
            };
        } 
    
        static inline constexpr TaskStatus Retry(Time::Duration waitDuration){
            return TaskStatus{
                TaskStatus::RETRY,
                RetryStatus{waitDuration}
            };
        }

        
        static inline constexpr TaskStatus Failed(const std::string& reason = {}){
            return TaskStatus{
                TaskStatus::FAILED,
                FailedStatus{reason}
            };
        }
    };
}