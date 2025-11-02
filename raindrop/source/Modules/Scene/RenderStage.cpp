#include "Raindrop/Modules/Scene/RenderStage.hpp"
#include "Raindrop/Modules/Scene/SceneTrait.hpp"

namespace Raindrop::Scene{

    RenderStage::RenderStage(Layers::Layer layer, std::shared_ptr<Render::IRenderOutput> output, std::shared_ptr<Render::RenderCommandContext> cmdCtx) : 
        _layer{layer},
        _output{output},
        _cmdCtx(cmdCtx)
    {

    }

    const char* RenderStage::name() const{
        return "Scene - render stage";
    }

    void RenderStage::initialize(Scheduler::StageInitHelper& helper){

    }

    void RenderStage::shutdown(){

    }

    Scheduler::StageResult RenderStage::execute(){

        if (!_layer.isValid()){
            return Scheduler::StageResult::Skip("Layer not valid");
        }

        auto output = _output.lock();

        if (!output){
            return Scheduler::StageResult::Skip("No output");
        }

        if (!output->wasAcquired()){
            return Scheduler::StageResult::Skip("Output was not acquired");
        }

        auto cmdCtx = _cmdCtx.lock();

        if (!cmdCtx){
            return Scheduler::StageResult::Skip("no render command context");
        }


        auto& scene = _layer.getTraits<SceneTrait>().scene;

        

        return Scheduler::StageResult::Continue();
    }
}