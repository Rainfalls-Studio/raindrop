#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Task.hpp"
#include "TaskProfile.hpp"

namespace Raindrop::Tasks{
    class TaskManager : public std::enable_shared_from_this<TaskManager> {
        public:
            TaskManager(uint32_t workerCount = static_cast<uint32_t>(std::thread::hardware_concurrency()));
            ~TaskManager();

            // Create a task; returns its id
            uint64_t createTask(Task::Func function,
                                int priority = 0,
                                const std::vector<uint64_t>& dependencies = {},
                                std::string name = "");

            // Query profiling data for a task by id
            std::shared_ptr<TaskProfile> getProfile(uint64_t id);

            // Wait until all tasks that currently exist are finished
            void waitForAll();

            // Graceful shutdown: stop accepting tasks and join workers.
            void shutdown();

        private:
            struct TaskEntry {
                std::shared_ptr<Task> task; // may be null for placeholders
                std::atomic<int> unmetDeps{0};
                std::vector<uint64_t> dependents; // task ids that depend on this one
                std::atomic<bool> completed{false};
                std::shared_ptr<TaskProfile> profile = std::make_shared<TaskProfile>();
            };
            
            struct ReadyItem {
                std::shared_ptr<TaskEntry> entry;
                TimePoint availableAt;
            };

            struct ReadyComparator {
                // higher priority runs first; tie-breaker older insertion (by id)
                bool operator()(const ReadyItem& a, const ReadyItem& b) const {
                    if (a.entry->task->priority != b.entry->task->priority)
                        return a.entry->task->priority < b.entry->task->priority;
                    return a.entry->task->id > b.entry->task->id;
                }
            };
            
            // internal storage
            std::mutex mutex;
            std::condition_variable cv;
            std::condition_variable finishedCv;
            std::unordered_map<uint64_t, std::shared_ptr<TaskEntry>> tasks;
            std::priority_queue<ReadyItem, std::vector<ReadyItem>, ReadyComparator> readyPQ;
            std::vector<std::thread> workers;
            std::atomic<bool> running{false};
            std::atomic<uint64_t> nextId{1};

            // push ready task (available now)
            void pushReady(const std::shared_ptr<TaskEntry>& entry, TimePoint when = Clock::now());

            // worker loop
            void workerLoop();
    };
}  // namespace Raindrop::Tasks