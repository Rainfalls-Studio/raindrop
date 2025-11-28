#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <list>

#include "TaskHandle.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Tasks{
    class TaskManager {
        friend class ::Raindrop::Engine;
        public:
            TaskManager(Engine& engine, unsigned workers = std::thread::hardware_concurrency());
            ~TaskManager();

            TaskHandle createTask(std::function<TaskStatus()> fn, Priority priority = Priority::MEDIUM, std::string name = "");

            void submit(const TaskHandle& task);
            void shutdown();
            void stop();

            size_t taskCount();

        private:
            // A simple task that has been registred and is waiting to run
            struct TaskInstance {
                std::shared_ptr<TaskHandle::TaskDef> ref;
            };
            
            // A task that has been ask to be delayed and will run from a specific time
            struct WaitingTask{
                TaskInstance instance;
                Time::TimePoint availability;
            };

            using TaskPool = std::array<std::list<TaskInstance>, static_cast<size_t>(Priority::__size__)>;
            
            Engine& _engine;
            std::mutex mtx;
            std::condition_variable cv;
            TaskPool _tasks;
            std::list<WaitingTask> _waiting;
            std::vector<std::thread> threads;
            std::atomic<bool> running{false};

            void workerLoop();
            TaskInstance nextTask(std::unique_lock<std::mutex>& lock);

        };
}  // namespace Raindrop::Tasks