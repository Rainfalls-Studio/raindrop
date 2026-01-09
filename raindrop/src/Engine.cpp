#include "Raindrop/Engine.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop{

    Engine::Engine() : 
        _modules(*this)
    {}

    ModuleManager& Engine::getModuleManager() noexcept{
        return _modules;
    }

    void Engine::initialize(){
        _modules.initialize();
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