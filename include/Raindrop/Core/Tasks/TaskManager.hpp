#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "TaskHandle.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Tasks{
    class TaskManager {
        public:
            TaskManager(Engine& engine, unsigned workers = std::thread::hardware_concurrency());
            ~TaskManager();

            TaskHandle createTask(std::function<TaskStatus()> fn, int priority = 0, std::string name = "");

            void submit(const TaskHandle& task);
            void shutdown();

        private:
            struct TaskInstance {
                std::shared_ptr<TaskHandle::TaskDef> def;
                std::atomic<int> unmetDeps{0};
            };

            struct Compare {
                bool operator()(const std::shared_ptr<TaskInstance>& a,
                                const std::shared_ptr<TaskInstance>& b) const {
                    if (a->def->priority != b->def->priority)
                        return a->def->priority < b->def->priority;
                    return a < b; // arbitrary tie-break
                }
            };
            
            Engine& _engine;
            std::mutex mtx;
            std::condition_variable cv;
            std::priority_queue<std::shared_ptr<TaskInstance>, std::vector<std::shared_ptr<TaskInstance>>, Compare> ready;
            std::vector<std::thread> threads;
            std::atomic<bool> running{false};

            void workerLoop();
        };
}  // namespace Raindrop::Tasks