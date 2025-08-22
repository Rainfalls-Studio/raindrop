#pragma once

#include "Raindrop/Core/Time/Clock.hpp"

namespace Raindrop::Scheduler{
    struct ContinueStatus{};

    struct HookRetryStatus{
        Time::Duration waitDuration;
    };

    struct SkipStatus{
        std::string reason;
    };

    struct HookResult{
        enum Type {
            CONTINUE,
            RETRY_HOOK,
            SKIP
        } type = CONTINUE;

        std::variant<
            ContinueStatus,
            HookRetryStatus,
            SkipStatus
        > status;

        inline ContinueStatus& getContinue(){
            return std::get<ContinueStatus>(status);
        }

        inline HookRetryStatus& getRetryHook(){
            return std::get<HookRetryStatus>(status);
        }

        inline SkipStatus& getSkip(){
            return std::get<SkipStatus>(status);
        }

        static inline constexpr HookResult Continue(){
            return HookResult{
                CONTINUE,
                ContinueStatus{}
            };
        } 
    
        static inline constexpr HookResult Retry(Time::Duration waitDuration){
            return HookResult{
                RETRY_HOOK,
                HookRetryStatus{waitDuration}
            };
        }

        
        static inline constexpr HookResult Skip(const std::string& reason = {}){
            return HookResult{
                SKIP,
                SkipStatus{reason}
            };
        }
    };
}