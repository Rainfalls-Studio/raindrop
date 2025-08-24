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
    RenderGraphModule::RenderGraphModule(){}
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

    std::shared_ptr<crg::FrameGraph> RenderGraphModule::createGraph(const std::string& name){
        spdlog::trace("Creating frame graph \"{}\"...", name);

        std::shared_ptr<crg::FrameGraph> graph = std::make_shared<crg::FrameGraph>(*_resourceHandler, name);
        return graph;
    }

    Modules::Result RenderGraphModule::buildContext(){
        spdlog::trace("Building frame graph context...");
        try{
            _context = std::make_unique<crg::GraphContext>(
                _core->device(),
                VK_NULL_HANDLE,
                nullptr,
                _core->physicalDevice().getMemoryProperties(),
                _core->physicalDevice().getProperties(),
                false,
                &vkGetDeviceProcAddr
            );
        } catch (std::exception &e){
            return Modules::Result::Error(e.what());
        }

        return Modules::Result::Success();
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
        _core->device().waitIdle();
        
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
}