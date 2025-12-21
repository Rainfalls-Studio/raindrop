#include "Tasks/TaskManager.hpp"
#include <spdlog/spdlog.h>
#include <Raindrop/Modules/InitHelper.hpp>

constexpr static Raindrop::Time::Duration safetyMargin = Raindrop::Time::nanoseconds(500);


extern "C" RAINDROP_EXPORT Raindrop::Modules::IModule* CreateModule(){
	return new Raindrop::Tasks::TaskManager();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::Modules::IModule* module){
	delete module;
}


namespace Raindrop::Tasks{
    TaskManager::TaskManager(){}

    TaskManager::~TaskManager() {}

    Modules::Result TaskManager::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();

        size_t workers = std::thread::hardware_concurrency() - 2;

        running.store(true);
        for (unsigned i = 0; i < workers; ++i){
            threads.emplace_back(&TaskManager::workerLoop, this);
        }

        return Modules::Result::Success();
    }


    TaskHandle TaskManager::createTask(std::function<TaskStatus()> fn, Priority priority, std::string name) {
        return TaskHandle(std::make_shared<TaskHandle::TaskDef>(std::move(fn), priority, std::move(name), TaskProfile{}));
    }

    void TaskManager::stop(){
        running.store(false);
        cv.notify_all();
    }

    void TaskManager::submit(const TaskHandle& h) {
        if (!h.def) return;

        TaskInstance inst(h.def);

        {
            std::lock_guard<std::mutex> lock(mtx);
            _tasks[static_cast<size_t>(h->priority)].push_back(inst);
        }
        cv.notify_one();
    }

    void TaskManager::shutdown() {
        spdlog::info("Shutting down task manager...");
        stop();
        for (auto& t : threads) if (t.joinable()) t.join();

        for (auto& q : _tasks){
            q.clear();
        }

        _waiting.clear();
        threads.clear();
    }

    TaskManager::TaskInstance TaskManager::nextTask(std::unique_lock<std::mutex>& lock){

        while (true){
            const Time::TimePoint now = Time::now();

            Time::nanoseconds remaningNsUntilNextWaitingTask = Time::nanoseconds::max();
            auto currentWaitingTask = _waiting.end();


            {
                lock.lock();

                for (auto it=_waiting.begin(); it!=_waiting.end(); ++it){
                    auto remaning = (it->availability - now).as<Time::nanoseconds>();

                    if (remaning < remaningNsUntilNextWaitingTask){
                        remaningNsUntilNextWaitingTask = remaning;
                        currentWaitingTask = it;
                    }
                }
                
                // if the task scheduled time, just run now
                if (safetyMargin >= remaningNsUntilNextWaitingTask){
                    auto instance = currentWaitingTask->instance;
                    _waiting.erase(currentWaitingTask);

                    lock.unlock();
                    return instance;
                }

                // lock.unlock();
            }

            // wait either for a waiting task or one in the pool
            cv.wait_for(
                lock,
                remaningNsUntilNextWaitingTask
            );


            if (!running.load()){
                lock.unlock();
                return TaskInstance{nullptr};
            }

            {
                // lock.lock();

                // reverse to account for higher to lower priority
                for (TaskPool::reverse_iterator it = _tasks.rbegin(); it != _tasks.rend(); ++it){
                    if (it->empty()) continue;
                    
                    auto taskIt = it->begin();
                    auto task = *taskIt;
                    it->erase(taskIt);

                    lock.unlock();
                    return task;
                }

                lock.unlock();
            }
        }
    }

    void TaskManager::workerLoop() {
        while (running.load()) {
            TaskInstance inst;
            std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
            {
                inst = nextTask(lock);

                if (inst.ref == nullptr){
                    continue;;
                }
            }

            // spdlog::trace("TaskManager::workerLoop: Running task {}", inst.ref->name);

            auto start = Time::now();
            TaskStatus res = TaskStatus::Failed();

            try {
                res = inst.ref->fn();
            } catch (const std::exception& e) {
                res = TaskStatus::Failed(e.what());
            }

            auto end = Time::now();
            auto duration = end - start;

            inst.ref->profile.addRun(duration);
            
            if (res.type == TaskStatus::RETRY) {
                lock.lock();
                _waiting.push_back({inst, Time::TimePoint(end + res.getRetry().waitDuration.as<Time::nanoseconds>())});
                lock.unlock();
                continue;
            }

            if (res.type == TaskStatus::COMPLETED) {
                // Auto-submit chained tasks
                if (auto next = inst.ref->chained){
                    submit(TaskHandle(next));
                }
                continue;
            }

            if (res.type == TaskStatus::FAILED){
                spdlog::error("Task \"{}\" failed ! :: {}", inst.ref->name, res.getFailed().reason);
            }
        }
    }

    size_t TaskManager::taskCount(){
        std::lock_guard<std::mutex> lock(mtx);
        size_t sum=0;

        // cummulative sum of all the queues
        for (const auto& q : _tasks){
            sum+=q.size();
        }
        return sum;
    }

}