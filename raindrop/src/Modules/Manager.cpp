#include "Raindrop/Modules/Manager.hpp"
#include "Raindrop/Modules/InitHelper.hpp"
#include "Raindrop/Modules/Loaders/DynamicModuleLoader.hpp"

#include <cassert>
#include <spdlog/spdlog.h>
#include <queue>

#include <filesystem>
namespace fs = std::filesystem;

namespace Raindrop::Modules{
    Manager::Manager(Engine& engine) : _engine{engine}{}
    
    Manager::~Manager(){
        shutdown();
    }

    void Manager::loadModules(const std::filesystem::path& directoryPath){
        spdlog::info("Loading modules from \"{}\"", directoryPath.string());

        for (const auto & entry : fs::directory_iterator(directoryPath)){
            if (!entry.is_directory()) continue;
            loadModule(entry.path());
        }
    }

    void Manager::loadModule(const std::filesystem::path& path){
        try{
            registerModule(std::make_unique<DynamicModuleLoader>(path));
        } catch (const std::exception& e){
            spdlog::error("Failed to load module at path \"{}\" : {}", path.string(), e.what());
        }
    }

    void Manager::registerModule(std::unique_ptr<IModuleLoader>&& loader){
        std::unique_lock<std::mutex> lock(_mtx);

        std::shared_ptr<IModule> module = loader->create();

        // lock.lock();
        // TODO: module override

        auto& node = _nodes[module->name()];

        node.module = module;
        node.status = Status::NEW;
        node.loader = std::move(loader);

        std::string nodeName = node.name();

        // unregister previous dependencies
        for (const auto& dependency : node.dependencies){
            auto& depNode = _nodes[dependency.get()];
            depNode.dependents.remove(nodeName);
        }
        node.dependencies.clear();

        // check if dependencies are met and push dependencies
        bool dependenciesRequirements = true;

        for (const auto& dependency : module->dependencies()){
            auto& depNode = _nodes[dependency.get()];
            node.dependencies.push_back(dependency);
            depNode.dependents.push_back(nodeName);

            if (depNode.status != Status::INITIALIZED){
                dependenciesRequirements = false;
            }
        }

        // if dependencies are met. initialize module
        if (dependenciesRequirements){
            initializeModule(node);
        }

        if (node.module->critical() && node.status != Status::INITIALIZED){
            spdlog::critical("Failed to initialize critical module \"{}\" !", nodeName);
            throw std::runtime_error("Failed to initialize critical module");
        }
        
        propagateInitialization(node);
    }

    void Manager::propagateInitialization(Manager::Node& source){
        struct QueueData{
            Name parentName;
            Name currentName;

            inline Node& parent(Map& map) const noexcept{
                return map[parentName];
            }

            inline Node& current(Map& map) const noexcept{
                return map[currentName];
            }
        };

        std::queue<QueueData> queue;

        for (const auto& dep : source.dependents){
            queue.emplace(source.name(), dep);
        }

        while (!queue.empty()){
            QueueData data = queue.front();
            queue.pop();

            Node& current = data.current(_nodes);
            Node& parent = data.parent(_nodes);

            // if (currentNode.version == source.version){
            //     continue;
            // }

            // current.version = source.version;

            // if initialized, propagate initialization. Else propagate failure
            if (parent.status == Status::INITIALIZED){

                // if initialized, reload
                if (current.status == Status::INITIALIZED){
                    Result result = current.module->dependencyReload(data.parentName);
                    current.status = catchResultError(data.currentName, result);
                
                // if not initialized, initialize
                } else {
                    initializeModule(current);
                    // current.version = parent.version;
                }
            } else {
                // if initialized, signal shutdown
                if (current.status == Status::INITIALIZED){
                    Result result = current.module->dependencyShutdown(data.parentName);
                    current.status = catchResultError(data.currentName, result);
                }

                // if not, do nothing
            }

            // if the status is not initialized, reset module
            if (current.status != Status::INITIALIZED){
                // current.module = nullptr;
            }

            // add dependents
            for (const auto& dep : current.dependents){
                queue.emplace(current.name(), dep);
            }
        }
    }

    bool Manager::areModuleDependenciesMet(Node& node){
        for (const auto& dep : node.dependencies){
            const auto& depNode = _nodes[dep.get()];
            if (depNode.status != Status::INITIALIZED){
                return false;
            }
        }

        return true;
    }

    void Manager::initializeModule(Node& node){
        Result result = Result::Level::ERROR;
        SharedModule module = node.module;
        Name nodeName = node.name();

        ModuleMap dependencies;

        for (auto & dependency : node.dependencies){
            Name depName = dependency.get();
            Node& depNode = _nodes[depName];

            dependencies[depName] = depNode.status == Status::INITIALIZED ? depNode.module : nullptr;
        }

        InitHelper helper(_engine, dependencies);

        spdlog::trace("initializing node \"{}\"...", nodeName);

        // node.version = _version;
        result = module->initialize(helper);
        node.status = catchResultError(nodeName, result);
    }

    Status Manager::catchResultError(const Name& name, const Result& result){

        const std::string result_msg = result.message().empty() ? "no message" : result.message();

        // catch result
        switch (result.level()){
            case Result::Level::SUCCESS:{
                spdlog::info("Successfully initialized module \"{}\" : {} !", name, result_msg);
                return Status::INITIALIZED;
            }

            case Result::Level::ERROR:{
                spdlog::warn("Failed to initialize module \"{}\" : {}", name, result_msg);
                return Status::FAILED;
            }

            case Result::Level::FATAL:{
                spdlog::critical("Failed to initialize critical module \"{}\" : {}", name, result_msg);
                throw std::runtime_error(result.message());
            }
        }

        spdlog::warn("Unknown result level : {} : {}", static_cast<int>(result.level()), result_msg);
        return Status::FAILED;
    }

    SharedModule Manager::getModule(const std::string& name) noexcept{
        auto& node = _nodes[name];

        if (node.status == Status::INITIALIZED){
            return node.module;
        }

        return {};
    }

    void Manager::shutdown(){
        spdlog::info("Shuting down modules...");

        std::queue<Name> queue;

        // push leafs
        for (auto& [name, node] : _nodes){
            if (node.dependents.size() == 0){
                queue.push(name);
            }
        }

        // for each nodes
        while (!queue.empty()){
            Name name = queue.front();
            queue.pop();

            Node& node = _nodes[name];

            // if node has still dependents, skip it.
            if (!node.dependents.empty()) continue;

            // if not, reset
            if (node.module){
                spdlog::info("Shuting down module \"{}\"...", node.name());
                node.module->shutdown();
            }
            
            node.module = nullptr;

            // push dependents and remove self from dependents
            for (auto& dep : node.dependencies){
                auto& depNode = _nodes[dep.get()];

                depNode.dependents.remove(name);
                queue.push(dep.get());
            }
        }
    }
}