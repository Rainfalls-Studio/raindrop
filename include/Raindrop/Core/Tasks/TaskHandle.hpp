#pragma once

#include <functional>
#include <memory>
#include "Raindrop/Core/Time/Clock.hpp"
#include "TaskProfile.hpp"
#include "fwd.hpp"
#include "TaskStatus.hpp"

namespace Raindrop::Tasks{
    class TaskHandle {
        friend class TaskManager;
        public:
            struct TaskDef {
                std::function<TaskStatus()> fn;
                int priority = 0;
                std::vector<std::weak_ptr<TaskDef>> chained;
                std::string name;
                TaskProfile profile;
                Time::Duration defaultRetryDelay = Time::milliseconds(1);

                TaskDef(
                    const std::function<TaskStatus()>& fn_,
                    int priority_,
                    std::vector<std::weak_ptr<TaskDef>> chained_,
                    std::string name_,
                    TaskProfile profile_,
                    Time::Duration defaultRetryDelay_ = Time::milliseconds(1)
                ) : 
                    fn(std::move(fn_)),
                    priority(priority_),
                    chained(std::move(chained_)),
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