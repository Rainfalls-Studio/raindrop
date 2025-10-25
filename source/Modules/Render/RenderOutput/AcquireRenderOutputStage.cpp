#include "Raindrop/Modules/Render/RenderOutput/AcquireRenderOutputStage.hpp"
#include <spdlog/spdlog.h>
#include "Raindrop/Modules/Render/RenderOutput/RenderOutputModule.hpp"
#include "Raindrop/Modules/Render/Stages/RenderInfo.hpp"
#include "Raindrop/Engine.hpp"

namespace Raindrop::Render{
    AcquireRenderOutputStage::AcquireRenderOutputStage(const std::string& outputName) : _outputName(outputName){}

    void AcquireRenderOutputStage::findOutput(){
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

    const char* AcquireRenderOutputStage::name() const{
        return "AcquireRenderOutput";
    }

    void AcquireRenderOutputStage::initialize(Scheduler::StageInitHelper& helper){
        using namespace Scheduler;

        _loop = helper.loop();
        _engine = &helper.engine();

        findOutput();
    }

    void AcquireRenderOutputStage::shutdown(){}

    Scheduler::StageResult AcquireRenderOutputStage::execute(){
        using namespace Scheduler;

        auto output = _output.lock();
        if (!output){
            spdlog::error("No render output !");
            return StageResult::Skip("No render output");
        }

        auto& renderInfo = _loop.getOrEmplaceStorage<RenderInfo>();
        renderInfo.available = false;


        // Acquire
        {
            auto result = output->acquire(renderInfo.renderFinishedFence);
            
            if (!result){
                renderInfo.available = false;
                const auto& error = result.error();
                spdlog::error("Failed to acquire render output \"{}\" : {} :: {}", _outputName, error.message(), error.reason());
                return StageResult::Skip("Failed to acquire render output");
            }

            renderInfo.imageAvailable = result.value();
            const bool acquired = renderInfo.imageAvailable != VK_NULL_HANDLE;
            renderInfo.available = acquired;

            // spdlog::info(renderInfo.available);

            if (!acquired){
                return StageResult::Skip("Failed to acquire render output");
            }
        }
        
        // Pre render
        {
            auto result = output->preRender();

            if (!result){
                const auto& error = result.error();
                spdlog::error("Failed to pre render render output \"{}\" : {} :: {}", _outputName, error.message(), error.reason());
                return StageResult::Skip("Failed to pre render render output");
            }
        }


        // auto rl = output->resources()->blocking_acquire_read_latest();
        
        renderInfo.currentFrame = output->getCurrentBufferIndex();
        renderInfo.frameCount = output->getBufferCount();

        // renderInfo.imageAvailable = rl->imageAvailableSemaphore;
        // renderInfo.imageAvailableWaitStageFlags = vk::PipelineStageFlagBits::eBottomOfPipe;

        return StageResult::Continue();
    }
}