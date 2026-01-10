#include "Raindrop/Modules/Instances/DynamicModuleInstance.hpp"
#include "Raindrop/Modules/IModule.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <spdlog/spdlog.h>
#include <fstream>

#ifdef _WIN32
    #error To implement
#else
    #include <dlfcn.h>
#endif

namespace Raindrop{
    DynamicModuleInstance::DynamicModuleInstance(const std::filesystem::path& path) : _path{path}{
        readManifest(path);
    }

    DynamicModuleInstance::~DynamicModuleInstance(){
    }

    void DynamicModuleInstance::readManifest(const std::filesystem::path& path){
        std::ifstream stream(path / "manifest.json");

        if (!stream){
            throw std::runtime_error("Failed to read file");
        }

        json manifest = json::parse(stream);

        _name = manifest["name"].get<std::string>();
        _description = manifest["description"].get<std::string>();
        _critical = manifest["critical"].get<bool>();

        try{
            _version = Version::parse(manifest["version"].get<std::string>());
        } catch (const std::exception &e){
            spdlog::error("Failed to read version of module \"{}\" from manifest file : {}", _name, e.what());
            throw;
        }
        
        auto dependenciesNode = manifest["dependencies"];
        for (const auto& [name, constraint] : dependenciesNode.items()){
            _dependencies.push_back(
                Dependency(name, VersionConstraint::Parse(constraint.get<std::string>()))
            );
        }

        auto binaries = manifest["binaries"];

        #ifdef _WIN32
            _libraryPath = binaries["windows"].get<std::string>();
        #elif defined(__APPLE__)
            _libraryPath = binaries["apple"].get<std::string>();
        #elif defined(__linux__)
            _libraryPath = binaries["linux"].get<std::string>();
        #else
            #error Unsupported platform !
        #endif

        // spdlog::info(" a : {}", manifest["name"].get<std::string>());
        spdlog::info("Found module {} - version {}", _name, _version);
    }


    const DependencyList& DynamicModuleInstance::dependencies() const{
        return _dependencies;
    }

    const Name& DynamicModuleInstance::name() const{
        return _name;
    }

    const Version& DynamicModuleInstance::version() const{
        return _version;
    }

    bool DynamicModuleInstance::critical() const{
        return _critical;
    }

    Result DynamicModuleInstance::readLibrary(){

        std::filesystem::path path = _path / _libraryPath;
        
        #ifdef _WIN32

        #else
            _handle = dlopen(path.string().c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);

            if (!_handle){
                const char* error = dlerror();
                spdlog::error("Failed to load library: {}", path.string());
                spdlog::error("Error: {}", (error ? error : "Unknown error"));
                
                // Additional diagnostics
                if (!std::filesystem::exists(path)) {
                    spdlog::error("Library file does not exist!");
                }

                return Result::Error("Failed to load module library");
            }
        #endif

        _create = reinterpret_cast<IModule*(*)()>(getSymbol("CreateModule"));
        if (!_create){
            return Result::Error("Failed to find symbol \"CreateModule\"");
        }

        _destroy = reinterpret_cast<void(*)(IModule*)>(getSymbol("DestroyModule"));
        if (!_destroy){
            return Result::Error("Failed to find symbol \"DestroyModule\"");
        }

        return Result::Success();
    }

    Result DynamicModuleInstance::initialize(InitHelper& helper){
        if (auto result = readLibrary(); result.level() != Result::Level::SUCCESS){
            return result;
        }

        auto module = _create();
        _module = std::shared_ptr<IModule>(module, _destroy);

        return _module->initialize(helper);
    }

    void DynamicModuleInstance::shutdown(){
        _module->shutdown();
        _module.reset();

        #ifdef _WIN32

        #else
            dlclose(_handle);
        #endif
    }


    std::shared_ptr<IModule> DynamicModuleInstance::module() const{
        return _module;
    }

    void* DynamicModuleInstance::getSymbol(const char* name){
        #ifdef _WIN32

        #else
            return dlsym(_handle, name);
        #endif
    }
}