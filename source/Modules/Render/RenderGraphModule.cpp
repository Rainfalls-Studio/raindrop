#include "Raindrop/Modules/Render/RenderGraphModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"

#include <RenderGraph/ResourceHandler.hpp>
#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/FrameGraph.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Render{
    RenderGraphModule::RenderGraphModule(){}
    RenderGraphModule::~RenderGraphModule(){}

    Modules::Result RenderGraphModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();
        _core = helper.getDependencyAs<RenderCoreModule>("RenderCore");

        buildContext();
        buildResourceHandler();
        buildFrameGraph();

        return Modules::Result::Success();
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

    void RenderGraphModule::buildFrameGraph(){
        spdlog::trace("Building frame graph...");
        _frameGraph = std::make_unique<crg::FrameGraph>(*_resourceHandler, "RenderGraph");
    }

    void RenderGraphModule::destroyFrameGraph(){
        _frameGraph.reset();
    }

    void RenderGraphModule::destroyContext(){
        _context.reset();
    }

    void RenderGraphModule::destroyResourceHandler(){
        _resourceHandler.reset();
    }
    
    void RenderGraphModule::shutdown(){
        destroyFrameGraph();
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

    crg::FrameGraph* RenderGraphModule::getFrameGraph(){
        return _frameGraph.get();
    }
}