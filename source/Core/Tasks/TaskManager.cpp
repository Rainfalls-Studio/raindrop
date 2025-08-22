#include "Raindrop/Core/Tasks/TaskManager.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Tasks{
    TaskManager::TaskManager(Engine& engine, unsigned workers) : _engine(engine) {
        if (!workers) workers = 1;

        running.store(true);
        for (unsigned i = 0; i < workers; ++i)
            threads.emplace_back(&TaskManager::workerLoop, this);
    }

    TaskManager::~TaskManager() { shutdown(); }

    TaskHandle TaskManager::createTask(std::function<TaskStatus()> fn, int priority, std::string name) {
        return TaskHandle(std::make_shared<TaskHandle::TaskDef>(std::move(fn), priority, std::move(name), TaskProfile{}));
    }

    void TaskManager::stop(){
        running.store(false);
        cv.notify_all();
    }

    void TaskManager::submit(const TaskHandle& h) {
        if (!h.def) return;
        auto inst = std::make_shared<TaskInstance>();
        inst->def = h.def;
        inst->unmetDeps.store(0);
        {
            std::lock_guard<std::mutex> lock(mtx);
            ready.push(inst);
        }
        cv.notify_one();
    }

    void TaskManager::shutdown() {
        stop();
        for (auto& t : threads) if (t.joinable()) t.join();
    }

     void TaskManager::workerLoop() {
        while (running.load()) {
            std::shared_ptr<TaskInstance> inst;
            {
                std::unique_lock<std::mutex> lock(mtx);
                if (ready.empty()) {
                    cv.wait(lock, [&] { return !running.load() || !ready.empty(); });
                    if (!running.load()) break;
                }
                inst = ready.top();
                ready.pop();
            }

            // spdlog::trace("TaskManager::workerLoop: Running task {}", inst->def->name);

            auto start = Time::now();
            TaskStatus res = TaskStatus::Failed();
            try { res = inst->def->fn(); } catch (...) {}
            auto end = Time::now();

            inst->def->profile.addRun(end - start);
            
            if (res.type == TaskStatus::RETRY) {
                std::this_thread::sleep_for(res.getRetry().waitDuration.as<Time::nanoseconds>());
                submit(TaskHandle(inst->def));
                continue;
            }

            if (res.type == TaskStatus::COMPLETED) {
                // Auto-submit chained tasks
                if (auto next = inst->def->chained){
                    submit(TaskHandle(next));
                }
            }
        }
    }
}