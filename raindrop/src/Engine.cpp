#include "Raindrop/Engine.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop{
    void printTitle(const char* title){
        spdlog::info("----------------------------------------------------------");
        spdlog::info("                         {}                           ", title);
        spdlog::info("----------------------------------------------------------");
    }

    Engine::Engine() : 
        _modules(*this),
        _scheduler(*this),
        _tasks(*this, 2)
    {
        printTitle("Startup");
    }

    void Engine::start(){
        printTitle("Runtime");

        _tasks.workerLoop();
        
        printTitle("Shutdown");

        _tasks.shutdown();
        _scheduler.shutdown();
        _modules.shutdown();

        printTitle("Stopped");
    }

    void Engine::stop(){
        _tasks.stop();
    }

    Modules::Manager& Engine::getModuleManager() noexcept{
        return _modules;
    }

    Scheduler::Scheduler& Engine::getScheduler() noexcept{
        return _scheduler;
    }

    Tasks::TaskManager& Engine::getTaskManager() noexcept{
        return _tasks;
    }

    std::filesystem::path Engine::executableDirectory(){
        char pBuf[256];
        std::filesystem::path path;
        size_t len = sizeof(pBuf); 

        #ifdef WIN32
            int bytes = GetModuleFileName(NULL, pBuf, len);
            path = pBuf;
        #else
            ssize_t bytes = std::min(readlink("/proc/self/exe", pBuf, len), ssize_t(len - 1));
            if(bytes >= 0)
                pBuf[bytes] = '\0';
            path = pBuf;
        #endif

        return path.parent_path();
    }
}