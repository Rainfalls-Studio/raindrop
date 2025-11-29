#pragma once

#include <functional>
#include <memory>
#include "Raindrop/Time/Clock.hpp"
#include "TaskProfile.hpp"
#include "fwd.hpp"
#include "TaskStatus.hpp"

namespace Raindrop::Tasks{
    enum class Priority{
        BACKGROUND,
        VERY_LOW,
        LOW,
        MEDIUM,
        HIGH,
        VERY_HIGH,
        CRITICAL,

        __size__
    };

    class TaskHandle {
        friend class TaskManager;
        public:
            struct TaskDef {
                std::function<TaskStatus()> fn;
                Priority priority = Priority::MEDIUM;
                std::shared_ptr<TaskDef> chained;
                std::string name;
                TaskProfile profile;
                Time::Duration defaultRetryDelay = Time::milliseconds(1);

                TaskDef(
                    const std::function<TaskStatus()>& fn_,
                    Priority priority_,
                    std::string name_,
                    TaskProfile profile_,
                    std::shared_ptr<TaskDef> next = {},
                    Time::Duration defaultRetryDelay_ = Time::milliseconds(1)
                ) : 
                    fn(std::move(fn_)),
                    priority(priority_),
                    chained(std::move(next)),
                    name(std::move(name_)),
                    profile(profile_),
                    defaultRetryDelay(defaultRetryDelay_)
                {}
            };

            TaskHandle() = default;
            TaskHandle then(const TaskHandle& next);

            inline TaskDef* operator->() const {
                return def.get();
            }

            inline TaskDef& operator*() const{
                return *def;
            }

            inline std::shared_ptr<TaskDef> definition() const{
                return def;
            }

        private:
            std::shared_ptr<TaskDef> def;
    
            explicit TaskHandle(std::shared_ptr<TaskDef> d);
    };
}