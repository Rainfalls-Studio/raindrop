#include "Raindrop/Modules/Render/RenderOutput/IRenderOutput.hpp"

namespace Raindrop::Render{

    IRenderOutput::BeginStage::BeginStage(std::shared_ptr<IRenderOutput> output, std::shared_ptr<RenderCommandContext> cmdCtx) :
        _output{output},
        _cmdCtx(cmdCtx)
    {}

    const char* IRenderOutput::BeginStage::name() const{
        return "Render Output - Begin stage";
    }

    Scheduler::StageResult IRenderOutput::BeginStage::execute(){
        auto output = _output.lock();

        if (!output){
            return Scheduler::StageResult::Skip("No output");
        }

        auto cmdCtx = _cmdCtx.lock();

        if (!cmdCtx){
            return Scheduler::StageResult::Skip("No render command context");
        }

        vk::CommandBuffer commandBuffer = cmdCtx->currentBuffer().cmdBuffer;
        output->begin(commandBuffer);

        return Scheduler::StageResult::Continue();
    }



    // ======================================================================


    IRenderOutput::EndStage::EndStage(std::shared_ptr<IRenderOutput> output, std::shared_ptr<RenderCommandContext> cmdCtx) :
        _output{output},
        _cmdCtx(cmdCtx)
    {}

    const char* IRenderOutput::EndStage::name() const{
        return "Render Output - End stage";
    }

    Scheduler::StageResult IRenderOutput::EndStage::execute(){
        auto output = _output.lock();

        if (!output){
            return Scheduler::StageResult::Skip("No output");
        }

        auto cmdCtx = _cmdCtx.lock();

        if (!cmdCtx){
            return Scheduler::StageResult::Skip("No render command context");
        }

        vk::CommandBuffer commandBuffer = cmdCtx->currentBuffer().cmdBuffer;
        output->end(commandBuffer);

        return Scheduler::StageResult::Continue();

    }




    // ------------------------------------------------------------------




    IRenderOutput::PresentStage::PresentStage(std::shared_ptr<IRenderOutput> output, std::shared_ptr<RenderCommandContext> cmdCtx) :
        _output(output),
        _cmdCtx(cmdCtx)
    {}

    const char* IRenderOutput::PresentStage::name() const{
        return "Render Output - Present stage";
    }

    void IRenderOutput::PresentStage::initialize(Scheduler::StageInitHelper& helper){
        using namespace Scheduler;
        _engine = &helper.engine();
    }

    void IRenderOutput::PresentStage::shutdown(){}

    Scheduler::StageResult IRenderOutput::PresentStage::execute(){
        using namespace Scheduler;

        auto output = _output.lock();
        if (!output){
            return StageResult::Skip("No render output");
        }

        auto cmdCtx = _cmdCtx.lock();
        if (!cmdCtx){
            return StageResult::Skip("No render command context");
        }

        if (output->wasAcquired()){
            vk::Semaphore semaphore = cmdCtx->currentBuffer().semaphore;

            auto result = output->present(semaphore);

            if (!result){
                const auto& error = result.error();
                return StageResult::Skip("Failed to present render output");
            }
        } else {
            return StageResult::Skip("The render output is not available");
        }

        return StageResult::Continue();
    }




    // ------------------------------------------------------------------




    IRenderOutput::AcquireStage::AcquireStage(std::shared_ptr<IRenderOutput> output, std::shared_ptr<RenderCommandContext> cmdCtx) :
        _output(output),
        _cmdCtx(cmdCtx)
    {}

    const char* IRenderOutput::AcquireStage::name() const{
        return "Render Output - Acquire stage";
    }

    void IRenderOutput::AcquireStage::initialize(Scheduler::StageInitHelper& helper){
        using namespace Scheduler;
        _engine = &helper.engine();
    }

    void IRenderOutput::AcquireStage::shutdown(){}

    Scheduler::StageResult IRenderOutput::AcquireStage::execute(){
        using namespace Scheduler;

        auto output = _output.lock();
        if (!output){
            return StageResult::Skip("No render output");
        }

        auto cmdCtx = _cmdCtx.lock();
        if (!cmdCtx){
            return StageResult::Skip("No render command context");
        }


        // Acquire
        {
            vk::Fence fence = cmdCtx->currentBuffer().fence;
            auto result = output->acquire(fence);
            
            if (!result){
                const auto& error = result.error();
                return StageResult::Skip("Failed to acquire render output - " + error.message() + " : " + error.reason());
            }
            
            if (!output->wasAcquired()){
                cmdCtx->skipFrame();
                return StageResult::Skip("Failed to acquire render output");
            }

            vk::Semaphore imageAvailableSemaphore = result.value();

            if (imageAvailableSemaphore)
                cmdCtx->waitFor(imageAvailableSemaphore);
            // else
                // cmdCtx->skipFrame();
        }


        // auto rl = output->resources()->blocking_acquire_read_latest();
        
        // renderInfo.currentFrame = output->getCurrentBufferIndex();
        // renderInfo.frameCount = output->getBufferCount();

        // renderInfo.imageAvailable = rl->imageAvailableSemaphore;
        // renderInfo.imageAvailableWaitStageFlags = vk::PipelineStageFlagBits::eBottomOfPipe;

        return StageResult::Continue();
    }
}