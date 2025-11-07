#include "Raindrop/Modules/Render/RenderGraph/RenderGraphModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"

#include <RenderGraph/ResourceHandler.hpp>
#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/FrameGraph.hpp>
#include <RenderGraph/RunnableGraph.hpp>
#include <RenderGraph/Log.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Raindrop::Render{
    RenderGraphModule::RenderGraphModule() : std::enable_shared_from_this<RenderGraphModule>(){}
    RenderGraphModule::~RenderGraphModule(){}

    Modules::Result RenderGraphModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();
        _core = helper.getDependencyAs<RenderCoreModule>("RenderCore");

        buildContext();
        buildResourceHandler();
        setupLogger();

        return Modules::Result::Success();
    }

    void RenderGraphModule::setupLogger(){
        _logger = spdlog::stdout_color_mt("RenderGraph");

        crg::Logger::setDebugCallback([this](const std::string& msg, bool){_logger->debug(msg);});
        crg::Logger::setTraceCallback([this](const std::string& msg, bool){_logger->trace(msg);});
        crg::Logger::setInfoCallback([this](const std::string& msg, bool){_logger->info(msg);});
        crg::Logger::setWarningCallback([this](const std::string& msg, bool){_logger->warn(msg);});
        crg::Logger::setErrorCallback([this](const std::string& msg, bool){_logger->error(msg);});
    }

    Modules::Result RenderGraphModule::buildContext(){
        spdlog::trace("Building frame graph context...");

        auto& deviceManager = _core->deviceManager();
        try{
            _context = std::make_unique<crg::GraphContext>(
                deviceManager.device(),
                VK_NULL_HANDLE,
                nullptr,
                deviceManager.physicalDevice().getMemoryProperties(),
                deviceManager.physicalDevice().getProperties(),
                false,
                &vkGetDeviceProcAddr
            );
        } catch (std::exception &e){
            return Modules::Result::Error(e.what());
        }

        return Modules::Result::Success();
    }

    std::shared_ptr<RenderGraph> RenderGraphModule::createGraph(const std::string& name){
        return std::make_shared<RenderGraph>(shared_from_this(), name);
    }

    void RenderGraphModule::buildResourceHandler(){
        spdlog::trace("Building frame graph resource handler...");
        _resourceHandler = std::make_unique<crg::ResourceHandler>();
    }

    void RenderGraphModule::destroyContext(){
        _context.reset();
    }

    void RenderGraphModule::destroyResourceHandler(){
        _resourceHandler.reset();
    }
    
    void RenderGraphModule::shutdown(){
        if (auto result = _core->deviceManager().device().waitIdle(); result != vk::Result::eSuccess){
            spdlog::error("Failed to wait for vk device to be idle : \"{}\"", vk::to_string(result));
        }
        
        destroyResourceHandler();
        destroyContext();
    }

    RenderGraphModule::Name RenderGraphModule::name() const noexcept{
        return "RenderGraph";
    }

    Modules::DependencyList RenderGraphModule::dependencies() const noexcept{
        return {
            Modules::Dependency("RenderCore")
        };
    }

    Modules::Result RenderGraphModule::dependencyReload(const Name& dep){
        if (dep == "RenderCore"){
            spdlog::info("Rebuilding frame graph context...");
            _core = _engine->getModuleManager().getModuleAs<RenderCoreModule>("RenderCore");
            return buildContext();
        }

        return Modules::Result::Success();
    }

    Modules::Result RenderGraphModule::dependencyShutdown(const Name& dep){
        if (dep == "RenderCore"){
            destroyContext();
            return Modules::Result::Error("RenderCore is a critical dependency of RenderGraph");
        }
        return Modules::Result::Success();
    }

    crg::GraphContext& RenderGraphModule::context(){
        return *_context;
    }

    crg::ResourceHandler& RenderGraphModule::resourceHandler(){
        return *_resourceHandler;
    }

    std::shared_ptr<RenderCoreModule> RenderGraphModule::core(){
        return _core;
    }
}