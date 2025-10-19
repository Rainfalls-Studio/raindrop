#pragma once

#include <memory>
#include <imgui.h>
#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>

#include <Raindrop/Modules/Render/RenderOutput/IRenderOutput.hpp>
#include <Raindrop/Core/Time/Clock.hpp>

namespace Raindrop::Render{
    class ImGuiService{
        public:
            struct Context{
                IRenderOutput::Name outputName;
                ImGuiContext* ctx;
                vk::DescriptorPool pool;
                std::weak_ptr<Raindrop::Render::IRenderOutput> output;

                // update time
                float timeScale = 1.f;
                float timePaused = false;
                Time::TimePoint last;
            };

            ImGuiService(std::shared_ptr<RenderCoreModule> core, std::shared_ptr<spdlog::logger> logger);
            ~ImGuiService();

            Context& getOrCreateContext(const IRenderOutput::Name& name, SharedRenderOutput output);
            void destroyContext(const IRenderOutput::Name& name);

            void begin(Context& context);
            void end(Context& context);

        private:
            std::shared_ptr<RenderCoreModule> _core;
            std::shared_ptr<spdlog::logger> _logger;
            std::unordered_map<IRenderOutput::Name, std::unique_ptr<Context>> _outputToContext;

            void setupBackend(Context& context, const SharedRenderOutput& output);
            vk::DescriptorPool createDescriptorPool();
            void destroyDescriptorPool(vk::DescriptorPool pool);

            void deleteContext(Context& context);
    };
}