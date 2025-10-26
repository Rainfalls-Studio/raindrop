#include "Raindrop/Modules/Render/RenderOutput/PresentRenderOutputStage.hpp"
#include <spdlog/spdlog.h>
#include "Raindrop/Modules/Render/RenderOutput/RenderOutputModule.hpp"
#include "Raindrop/Modules/Render/Stages/RenderInfo.hpp"
#include "Raindrop/Engine.hpp"

namespace Raindrop::Render{
    PresentRenderOutputStage::PresentRenderOutputStage(const std::string& outputName) : _outputName(outputName){}

    void PresentRenderOutputStage::findOutput(){
        auto outputs = _engine->getModuleManager().getModuleAs<RenderOutputModule>("RenderOutput");
        if (!outputs){
            spdlog::error("Cannot get output \"{}\", There is no \"RenderOutput\" module registred", _outputName);
            return;
        }

        _output = outputs->getOutput(_outputName);

        if (_output.expired()){
            spdlog::error("Output \"{}\" not found", _outputName);
        }
    }

    const char* PresentRenderOutputStage::name() const{
        return "PresentRenderOutput";
    }

    void PresentRenderOutputStage::initialize(Scheduler::StageInitHelper& helper){
        using namespace Scheduler;

        _loop = helper.loop();
        _engine = &helper.engine();

        findOutput();
    }

    void PresentRenderOutputStage::shutdown(){}

    Scheduler::StageResult PresentRenderOutputStage::execute(){
        using namespace Scheduler;

        auto output = _output.lock();

        if (!output){
            spdlog::error("No render output !");
            return StageResult::Skip("No render output");
        }
        
        auto& renderInfo = _loop.getOrEmplaceStorage<RenderInfo>();

        if (renderInfo.available){
            auto result = output->postRender(renderInfo.renderFinishedSemaphore);
            renderInfo.available = false;

            if (!result){
                const auto& error = result.error();
                spdlog::error("Failed to present render output \"{}\" : {} :: {}", _outputName, error.message(), error.reason());
                return StageResult::Skip("Failed to present render output");
            }
        } else {
            return StageResult::Skip("The render output is not available");
        }

        return StageResult::Continue();
    }
}