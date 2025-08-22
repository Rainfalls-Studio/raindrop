#include "Raindrop/Modules/Render/RenderOutput/PresentRenderOutputStage.hpp"
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

    void PresentRenderOutputStage::initialize(Scheduler::StageInitHelper& helper){
        using namespace Scheduler;

        _loop = helper.loop();
        _engine = &helper.engine();

        findOutput();

        helper.registerHook(Hook{
            Phase::PRE_RENDER,
            "Acquire render output : " + _outputName,

            [this]() -> HookResult {
                auto output = _output.lock();
                if (!output){
                    spdlog::error("No render output !");
                    return HookResult::Skip("No render output");
                }

                auto& renderInfo = _loop.getOrEmplaceStorage<RenderInfo>();
                renderInfo.available = false;

                // Acquire
                {
                    auto result = output->acquire();

                    if (!result){
                        const auto& error = result.error();
                        spdlog::error("Failed to aqcuire render output \"{}\" : {} :: {}", _outputName, error.message(), error.reason());
                        return HookResult::Skip("Failed to acquire render output");
                    }

                    bool acquired = *result;
                    renderInfo.available = acquired;

                    if (!acquired){
                        return HookResult::Skip("Failed to acquire render output");
                    }
                }
                
                // Pre render
                {
                    auto result = output->preRender();

                    if (!result){
                        const auto& error = result.error();
                        spdlog::error("Failed to pre render render output \"{}\" : {} :: {}", _outputName, error.message(), error.reason());
                        return HookResult::Skip("Failed to pre render render output");
                    }
                }

                return HookResult::Continue();
            }
        });

        helper.registerHook(Hook{
            Phase::POST_RENDER,
            "Present render output : " + _outputName,

            [this]() -> HookResult {
                auto output = _output.lock();

                if (!output){
                    spdlog::error("No render output !");
                    return HookResult::Skip("No render output");
                }
                
                auto& renderInfo = _loop.getOrEmplaceStorage<RenderInfo>();

                if (renderInfo.available){
                    auto result = output->postRender();

                    if (!result){
                        const auto& error = result.error();
                        spdlog::error("Failed to present render output \"{}\" : {} :: {}", _outputName, error.message(), error.reason());
                        return HookResult::Skip("Failed to present render output");
                    }
                } else {
                    return HookResult::Skip("The render output is not available");
                }

                return HookResult::Continue();
            }
        });
    }
}