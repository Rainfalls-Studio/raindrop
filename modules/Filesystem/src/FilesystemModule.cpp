#include "Filesystem/FilesystemModule.hpp"
#include "Raindrop/Modules/InitHelper.hpp"

#include <algorithm>

extern "C" RAINDROP_EXPORT Raindrop::Modules::IModule* CreateModule(){
	return new Raindrop::Filesystem::FilesystemModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::Modules::IModule* module){
	delete module;
}


namespace Raindrop::Filesystem{
    FilesystemModule::FilesystemModule(){}

    Modules::Result FilesystemModule::initialize(Modules::InitHelper&){
        return Modules::Result::Success();
    }

    void FilesystemModule::shutdown(){
        
    }

    Modules::Name FilesystemModule::name() const noexcept{
        return RAINDROP_CURRENT_MODULE_NAME;
    }

    MountTable& FilesystemModule::mountTable() noexcept{
        return _mounts;
    }

    bool FilesystemModule::exists(const Path& path) const{
        return _mounts.exists(path);
    }

    SharedFile FilesystemModule::open(const Path& path, OpenFlags flags) const{
        return _mounts.open(path, flags);
    }

    Path FilesystemModule::getExecutableDirectory(){
        char pBuf[256];
        size_t len = sizeof(pBuf);
        
        #ifdef __WIN32

            int bytes = GetModuleFileName(NULL, pBuf, len);
            return bytes ? bytes : -1;

        #endif

        #ifdef __unix__
            int bytes = std::min(static_cast<int>(readlink("/proc/self/exe", pBuf, len)), static_cast<int>(len - 1));
            if(bytes >= 0)
                pBuf[bytes] = '\0';
        #endif
        
        return Path(pBuf);
    }
}