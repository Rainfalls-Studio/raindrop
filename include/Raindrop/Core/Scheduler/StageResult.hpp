#pragma once

#include "Raindrop/Core/Time/Clock.hpp"

namespace Raindrop::Scheduler{
    struct ContinueStatus{};

    struct StageRetryStatus{
        Time::Duration waitDuration;
    };

    struct SkipStatus{
        std::string reason;
    };

    struct StageResult{
        enum Type {
            CONTINUE,
            RETRY_HOOK,
            SKIP
        } type = CONTINUE;

        std::variant<
            ContinueStatus,
            StageRetryStatus,
            SkipStatus
        > status;

        inline ContinueStatus& getContinue(){
            return std::get<ContinueStatus>(status);
        }

        inline StageRetryStatus& getRetry(){
            return std::get<StageRetryStatus>(status);
        }

        inline SkipStatus& getSkip(){
            return std::get<SkipStatus>(status);
        }

        static inline constexpr StageResult Continue(){
            return StageResult{
                CONTINUE,
                ContinueStatus{}
            };
        } 
    
        static inline constexpr StageResult Retry(Time::Duration waitDuration){
            return StageResult{
                RETRY_HOOK,
                StageRetryStatus{waitDuration}
            };
        }

        
        static inline constexpr StageResult Skip(const std::string& reason = {}){
            return StageResult{
                SKIP,
                SkipStatus{reason}
            };
        }
    };
}