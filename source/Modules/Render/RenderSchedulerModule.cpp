#include "Raindrop/Modules/Render/RenderSchedulerModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"

namespace Raindrop::Render{
    RenderSchedulerModule::RenderSchedulerModule(){}

    Modules::Result RenderSchedulerModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();
        _core = helper.getDependencyAs<RenderCoreModule>("RenderCore");

        auto& scheduler = helper.scheduler();

        _preRenderSubscription = scheduler.subscribe([this]{preRender();}, Scheduler::Priority::PRE_RENDER);
        _renderSubscription = scheduler.subscribe([this]{render();}, Scheduler::Priority::PRE_RENDER);
        _postRenderSubscription = scheduler.subscribe([this]{postRender();}, Scheduler::Priority::PRE_RENDER);

        return Modules::Result::Success();
    }

    void RenderSchedulerModule::shutdown(){
        _preRenderSubscription.unsubscribe();
        _renderSubscription.unsubscribe();
        _postRenderSubscription.unsubscribe();
    }

    RenderSchedulerModule::Name RenderSchedulerModule::name() const noexcept{
        return "RenderScheduler";
    }

    Modules::DependencyList RenderSchedulerModule::dependencies() const noexcept{
        return {
            Modules::Dependency("RenderCore")
        };
    }

    Modules::Result RenderSchedulerModule::dependencyReload(const Name& name){
        if (name == "RenderCore"){
            _core = _engine->getModuleManager().getModuleAs<RenderCoreModule>("RenderCore");
        }
        return Modules::Result::Success();
    }

    Modules::Result RenderSchedulerModule::dependencyShutdown(const Name& name){
        return Modules::Result::Error();
    }

    void RenderSchedulerModule::preRender(){
        if (_preparRender){
            _preparRender();
        }
    }

    void RenderSchedulerModule::render(){
        PreRenderResult preRenderResult;
        if (_preRender){
            preRenderResult = _preRender();
        }

        RenderResult renderResult;
        if (_render){
            renderResult = _render(preRenderResult);
        }

        if (_postRender){
            _postRender(renderResult);
        }
    }

    void RenderSchedulerModule::postRender(){
        
    }
}