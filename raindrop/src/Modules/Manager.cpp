#include "Raindrop/Modules/Manager.hpp"
#include "Raindrop/Modules/InitHelper.hpp"
#include "Raindrop/Modules/Instances/DynamicModuleInstance.hpp"

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
            registerModule(std::make_unique<DynamicModuleInstance>(path));
        } catch (const std::exception& e){
            spdlog::error("Failed to load module at path \"{}\" : {}", path.string(), e.what());
        }
    }

    void Manager::registerModule(std::unique_ptr<IModuleInstance>&& instance){
        std::unique_lock<std::mutex> lock(_mtx);

        // std::shared_ptr<IModule> module = loader->create();

        // lock.lock();
        // TODO: module override

        auto& node = _nodes[instance->name()];

        node.status = Status::NEW;
        node.instance = std::move(instance);
        auto& inst = node.instance;

        const Name& nodeName = node.name();

        // simply push as dependent of dependencies
        const DependencyList& dependencies = node.instance->dependencies();
        for (const auto& dependency : dependencies){
            auto& depNode = _nodes[dependency.get()];
            depNode.dependents.push_back(nodeName);
        }

        tryInitializeModule(node);

        if (inst->critical() && node.status != Status::INITIALIZED){
            spdlog::critical("Failed to initialize critical module \"{}\" !", nodeName);
            throw std::runtime_error("Failed to initialize critical module");
        }
    }

    void Manager::tryInitializeModule(Node& node){
        
        if (node.status == Status::INITIALIZED) return;
        auto& instance = node.instance;

        if (!instance) return;

        const Name& nodeName = instance->name();
        bool dependenciesAvailable = true;

        const DependencyList& dependencies = instance->dependencies();

        for (const auto& dependency : dependencies){
            auto& depNode = _nodes[dependency.get()];

            if (depNode.status != Status::INITIALIZED){
                dependenciesAvailable = false;
                spdlog::trace("The module {} is missing dependency {}", nodeName, dependency.get());
                break;
            }

            auto& depInstance = depNode.instance;

            if (!dependency.constraint().satisfies(depInstance->version())){
                dependenciesAvailable = false;

                auto& constraint = dependency.constraint();

                spdlog::trace("The module {} is expexting dependency {} with constraints {} but got version {}",
                    nodeName,
                    dependency.get(),
                    constraint.toString(),
                    depInstance->version()
                );

                break;
            }
        }

        if (!dependenciesAvailable){
            return;
        }


        initializeModule(node);

        if (node.status != Status::INITIALIZED) return;

        for (const auto& dep : node.dependents){
            tryInitializeModule(_nodes[dep]);
        }
    }


    void Manager::shutdownModule(Name module){
        auto& node = _nodes[module];
        shutdownModuleNode(node);
    }

    void Manager::shutdownModuleNode(Node& node){
        if (node.status != Status::INITIALIZED) return;
        node.status = Status::QUIESCING;

        auto& instance = node.instance;

        if (!instance){
            // technically a warning, but mark as error so dev can see
            spdlog::error("A module was marked as 'INITIALIZED' yet contained no valid instance");
            return;
        }

        for (const auto& dep : instance->dependencies()){
            shutdownModule(dep.get());
        }

        // finally shutdown
        node.instance->shutdown();
        node.status = Status::TERMINATED;
        node.instance.reset();
    }

    void Manager::initializeModule(Node& node){
        Result result = Result::Level::ERROR;
        auto& instance = node.instance;
        Name nodeName = node.name();

        ModuleMap dependencies;

        for (auto & dependency : instance->dependencies()){
            const auto& depName = dependency.get();
            Node& depNode = _nodes[depName];

            dependencies[depName] = depNode.instance->module();
        }

        InitHelper helper(_engine, dependencies);

        spdlog::trace("initializing node \"{}\"...", nodeName);

        // node.version = _version;
        result = instance->initialize(helper);
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
            return node.instance->module();
        }

        return {};
    }

    void Manager::shutdown(){
        spdlog::info("Shuting down modules...");

        std::queue<Name> queue;

        // push leafs
        for (auto& [name, node] : _nodes){
            if (node.status != Status::INITIALIZED) continue;

            auto& instance = node.instance;

            if (instance->dependencies().empty()){
                shutdownModuleNode(node);
            }
        }
    }
}