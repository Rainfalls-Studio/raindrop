#include "Raindrop/Modules/Render/ImGui/ImGuiRenderStep.hpp"

#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/ImageData.hpp>
#include <RenderGraph/RunnableGraph.hpp>

#include <imgui_impl_vulkan.h>
#include <imgui.h>

#include <spdlog/spdlog.h>

namespace Raindrop::Render{
	ImGuiRenderStep::ImGuiRenderStep(
        std::shared_ptr<ImGuiModule> module,
        SharedRenderOutput renderOutput,
        IRenderOutput::Name name
    ) :
        _module{module},
        _renderOutput{renderOutput},
        _name{name}
	{}

    void ImGuiRenderStep::record(
        vk::CommandBuffer cmd
    ){
        auto context = _module->service().getOrCreateContext(_name, _renderOutput);

        ImGui::SetCurrentContext(context.ctx);
        auto drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, cmd);
    }
}