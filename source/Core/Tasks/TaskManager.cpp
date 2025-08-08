#include "Raindrop/Core/Tasks/TaskManager.hpp"

namespace Raindrop::Tasks{
    TaskManager::TaskManager(uint32_t workerCount) {
        if (workerCount == 0) workerCount = 1;
        running.store(true);
        for (unsigned i = 0; i < workerCount; ++i) {
            workers.emplace_back(&TaskManager::workerLoop, this);
        }
    }

    TaskManager::~TaskManager() {
        shutdown();
    }

    uint64_t TaskManager::createTask(Task::Func fn,
                        int priority,
                        const std::vector<uint64_t>& deps,
                        std::string name)
    {
        auto id = nextId.fetch_add(1);
        auto entry = std::make_shared<TaskEntry>();
        entry->task = std::make_shared<Task>(id, priority, std::move(fn), deps, std::move(name));
        entry->unmetDeps.store(static_cast<int>(deps.size()));

        {
            std::lock_guard<std::mutex> lock(mutex);
            tasks[id] = entry;

            // register this task as a dependent of each dependency
            for (auto depId : deps) {
                auto it = tasks.find(depId);
                if (it == tasks.end()) {
                    // dependency not found: treat as immediate completion? safer: create a placeholder completed task
                    auto placeholder = std::make_shared<TaskEntry>();
                    placeholder->completed.store(true);
                    tasks[depId] = placeholder;
                    // nothing to do (no dependents list needed)
                }
                tasks[depId]->dependents.push_back(id);
            }

            if (entry->unmetDeps.load() == 0) {
                pushReady(entry);
            }
        }
        cv.notify_one();
        return id;
    }

    std::shared_ptr<TaskProfile> TaskManager::getProfile(uint64_t id) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = tasks.find(id);
        if (it == tasks.end() || !it->second->profile) return nullptr;
        return it->second->profile;
    }

    void TaskManager::waitForAll() {
        std::unique_lock<std::mutex> lock(mutex);
        finishedCv.wait(lock, [&]() {
            for (auto &kv : tasks) {
                if (!kv.second->completed.load()) return false;
            }
            return true;
        });
    }

    void TaskManager::shutdown() {
        bool expected = true;
        if (!running.compare_exchange_strong(expected, false)) return;
        cv.notify_all();
        for (auto &t : workers) if (t.joinable()) t.join();
    }

    void TaskManager::pushReady(const std::shared_ptr<TaskEntry>& entry, TimePoint when) {
        readyPQ.push(ReadyItem{entry, when});
    }

    void TaskManager::workerLoop() {
        while (running.load()) {
            std::shared_ptr<TaskEntry> entry;
            TimePoint wakeTime = TimePoint::max();
            {
                std::unique_lock<std::mutex> lock(mutex);
                // find an available ready task whose availableAt <= now
                while (running.load()) {
                    if (!readyPQ.empty()) {
                        auto item = readyPQ.top();
                        auto now = Clock::now();
                        if (item.availableAt <= now) {
                            entry = item.entry;
                            readyPQ.pop();
                            break;
                        } else {
                            // sleep until earliest availableAt or a new task arrives
                            wakeTime = item.availableAt;
                            break;
                        }
                    } else {
                        // no ready tasks: wait for notification
                        break;
                    }
                }

                if (!entry) {
                    if (!running.load()) break;
                    if (wakeTime == TimePoint::max()) {
                        cv.wait(lock);
                    } else {
                        cv.wait_until(lock, wakeTime);
                    }
                    continue;
                }
            }

            // run the task outside lock
            if (!entry || !entry->task) continue; // safety
            auto taskPtr = entry->task;
            auto start = Clock::now();
            TaskStatus res = TaskStatus::Failed;
            try {
                res = taskPtr->fn();
            } catch (...) {
                res = TaskStatus::Failed;
            }
            auto end = Clock::now();
            uint64_t ns = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
            entry->profile->addRun(ns);

            if (res == TaskStatus::Completed) {
                // mark completed and notify dependents
                std::vector<uint64_t> dependentsCopy;
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    if (!entry->completed.exchange(true)) {
                        dependentsCopy = entry->dependents;
                    }
                }
                // decrement unmetDeps for dependents
                if (!dependentsCopy.empty()) {
                    std::lock_guard<std::mutex> lock(mutex);
                    for (auto depId : dependentsCopy) {
                        auto it = tasks.find(depId);
                        if (it == tasks.end()) continue;
                        auto child = it->second;
                        int prev = child->unmetDeps.fetch_sub(1) ;
                        if (prev == 1) {
                            // now zero, schedule immediately
                            pushReady(child);
                            cv.notify_one();
                        }
                    }
                }
                // maybe signal finished
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    bool allDone = true;
                    for (auto &kv : tasks) {
                        if (!kv.second->completed.load()) { allDone = false; break; }
                    }
                    if (allDone) finishedCv.notify_all();
                }
            } else if (res == TaskStatus::Retry) {
                // schedule re-try without blocking: push back with delay (exponential backoff could be added)
                TimePoint when = Clock::now() + taskPtr->retryDelayOnRequest;
                std::lock_guard<std::mutex> lock(mutex);
                pushReady(entry, when);
                cv.notify_one();
            } else { // Failed
                // mark failed (still treat as completed for dependents? - choose: mark completed so dependents can proceed or fail)
                // Here: mark completed = true so dependents aren't blocked forever. Up to caller to check child results if needed.
                std::vector<uint64_t> dependentsCopy;
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    if (!entry->completed.exchange(true)) {
                        dependentsCopy = entry->dependents;
                    }
                }

                std::lock_guard<std::mutex> lock(mutex);
                for (auto depId : dependentsCopy) {
                    auto it = tasks.find(depId);
                    if (it == tasks.end()) continue;
                    auto child = it->second;
                    int prev = child->unmetDeps.fetch_sub(1);
                    if (prev == 1) {
                        pushReady(child);
                        cv.notify_one();
                    }
                }
            }
        }
    }
}