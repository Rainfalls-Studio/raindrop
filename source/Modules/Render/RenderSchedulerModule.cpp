#include "Raindrop/Modules/Render/RenderSchedulerModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"

namespace Raindrop::Render{
    RenderSchedulerModule::RenderSchedulerModule(){}

    Modules::Result RenderSchedulerModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();
        _core = helper.getDependencyAs<RenderCoreModule>("RenderCore");

        return Modules::Result::Success();
    }

    void RenderSchedulerModule::shutdown(){}

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
        if (name == "RenderCore"){
            return Modules::Result::Error();
        }
        return Modules::Result::Success();
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