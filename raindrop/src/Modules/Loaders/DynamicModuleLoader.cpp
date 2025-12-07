#include "Raindrop/Modules/Loaders/DynamicModuleLoader.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <spdlog/spdlog.h>

#include <fstream>
#include <cstring>

#ifdef _WIN32
    #error To implement
#else
    #include <dlfcn.h>
#endif

namespace Raindrop::Modules{
    DynamicModuleLoader::DynamicModuleLoader(const std::filesystem::path& path){
        // formtPath();
        std::filesystem::path libraryPath = path / readManifest(path);
        
        #ifdef _WIN32

        #else
            _handle = dlopen(libraryPath.string().c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);

            if (!_handle){
                const char* error = dlerror();
                spdlog::error("Failed to load library: {}", libraryPath.string());
                spdlog::error("Error: {}", (error ? error : "Unknown error"));
                
                // Additional diagnostics
                if (!std::filesystem::exists(libraryPath)) {
                    spdlog::error("Library file does not exist!");
                }

                throw std::runtime_error("Failed to load module loader");
            }
        #endif

        _create = reinterpret_cast<IModule*(*)()>(getSymbol("CreateModule"));

        if (!_create){
            throw std::runtime_error("Failed to get symbole \"CreateModule\"");
        }

        _destroy = reinterpret_cast<void(*)(IModule*)>(getSymbol("DestroyModule"));

        if (!_destroy){
            throw std::runtime_error("Failed to get symbole \"DestroyModule\"");
        }
    }

    DynamicModuleLoader::~DynamicModuleLoader(){
        #ifdef _WIN32

        #else
            dlclose(_handle);
        #endif
    }

    std::filesystem::path DynamicModuleLoader::readManifest(const std::filesystem::path& path){
        std::ifstream stream(path / "manifest.json");

        if (!stream){
            throw std::runtime_error("Failed to read file");
        }

        json manifest = json::parse(stream);

        auto name = manifest["name"].get<std::string>();
        auto description = manifest["description"].get<std::string>();
        Version version;

        try{
            version = Version::parse(manifest["version"].get<std::string>());
        } catch (const std::exception &e){
            spdlog::error("Failed to read version of module \"{}\" from manifest file : {}", name, e.what());
            throw;
        }
        
        auto dependenciesNode = manifest["dependencies"];

        auto binaries = manifest["binaries"];
        std::filesystem::path libraryPath;

        #ifdef _WIN32
            libraryPath = binaries["windows"].get<std::string>();
        #elif defined(__APPLE__)
            libraryPath = binaries["apple"].get<std::string>();
        #elif defined(__linux__)
            libraryPath = binaries["linux"].get<std::string>();
        #else
            #error Unsupported platform !
        #endif

        if (libraryPath.empty()){
            assert("aa");
        }

        // spdlog::info(" a : {}", manifest["name"].get<std::string>());
        spdlog::info("Found module {} - version {}", name, version.str());

        return libraryPath;
    }

    std::shared_ptr<IModule> DynamicModuleLoader::create(){
        return std::shared_ptr<IModule>(_create(), _destroy);
    }

    void* DynamicModuleLoader::getSymbol(const char* name){
        #ifdef _WIN32

        #else
            return dlsym(_handle, name);
        #endif
    }
}