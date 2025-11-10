#pragma once

#include <Raindrop/Raindrop.hpp>
#include <imgui_impl_vulkan.h>

class ContentStage : public Raindrop::Scheduler::IStage{
    public:
        ContentStage(std::shared_ptr<Raindrop::Render::BufferRenderOutput> output, std::shared_ptr<Raindrop::Scene::Scene> scene) :
            _output{output},
            _scene{scene}
        {}

        virtual ~ContentStage() override = default;

        virtual const char* name() const override{
            return "Content";
        }

        virtual void initialize(Raindrop::Scheduler::StageInitHelper& helper) override{
            _core = helper.modules().getModuleAs<Raindrop::Render::RenderCoreModule>("RenderCore");
            createSampler();
        }

        virtual void shutdown() override{
            auto device = _core->deviceManager().device();
            if (device.waitIdle() != vk::Result::eSuccess){
                spdlog::warn("Failed to wait for device idle");
            }

            if (_sampler){
                device.destroySampler(_sampler);
            }
        }

        virtual Raindrop::Scheduler::StageResult execute() override{
            ImGui::Begin("Content", nullptr,
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse
            );

            auto output = _output.lock();

            if (!output){
                ImGui::Text("Output not available !");
            } else {
                uint64_t outputEpoch = output->epoch();
                if (outputEpoch != _epoch){
                    auto imageView = output->currentColorImageView(0);

                    _descriptorSet = ImGui_ImplVulkan_AddTexture(_sampler, imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
                    _epoch = outputEpoch;
                }

                ImVec2 size = ImGui::GetWindowSize();
                auto textureSize = output->extent();

                if (static_cast<uint32_t>(size.x) != textureSize.width || static_cast<uint32_t>(size.y) != textureSize.height){
                    output->resize(vk::Extent2D{
                        static_cast<uint32_t>(size.x),
                        static_cast<uint32_t>(size.y)
                    });
                }

                if (_descriptorSet){
                    ImGui::SetCursorPos({0.f, 0.f});
                    ImGui::Image(reinterpret_cast<ImTextureID>(_descriptorSet), size);
                }
            }

            if (auto scene = _scene.lock()){

                auto entity = scene->registry().view<Raindrop::Components::Transform, Raindrop::Components::Camera>().front();
                if (scene->isValid(entity)){

                    const float sensibility = 1.f / ImGui::GetWindowSize().x;

                    auto [transform, camera] = scene->getComponent<Raindrop::Components::Transform, Raindrop::Components::Camera>(entity);
                    auto& translation = transform.translation;
                    auto& rotation = transform.rotation;
                    
                    {
                        auto size = ImGui::GetWindowSize();
                        camera.setSize(size.x, size.y);
                    }

                    float now = (Raindrop::Time::now().as<Raindrop::Time::milliseconds>().count() / 1000.f) / 5.f;

                    float d = glm::length(translation);
                    
                    if (ImGui::IsWindowHovered()){
                        float scroll = ImGui::GetIO().MouseWheel;
                        
                        if (glm::epsilonNotEqual(scroll, 0.f, 0.001f)){
                            d = glm::max(d + scroll, 1.f);
                        }
                    }

                    translation = {
                        glm::cos(now) * d,
                        0.f,
                        glm::sin(now) * d
                    };

                    rotation = glm::quatLookAt(
                        -glm::normalize(translation),
                        glm::vec3(0, 1, 0)
                    );

                    transform.worldTransform = 
                        glm::translate(glm::mat4(1.f), translation) *
                        glm::toMat4(rotation) *
                        glm::scale(glm::mat4(1.f), transform.scale);
                }
            }

            ImGui::End();
            return Raindrop::Scheduler::StageResult::Continue();
        }
    
    private:
        VkDescriptorSet _descriptorSet = {};
        vk::Sampler _sampler;
        uint64_t _epoch = 0;
        std::shared_ptr<Raindrop::Render::RenderCoreModule> _core;
        std::weak_ptr<Raindrop::Render::BufferRenderOutput> _output;
        std::weak_ptr<Raindrop::Scene::Scene> _scene;

        void createSampler(){
            vk::SamplerCreateInfo info{
                {},
                vk::Filter::eNearest,
                vk::Filter::eNearest,
                vk::SamplerMipmapMode::eNearest,
                vk::SamplerAddressMode::eRepeat,
                vk::SamplerAddressMode::eRepeat,
                vk::SamplerAddressMode::eRepeat,
                0.f,
                vk::False,
                0.f,
                vk::False,
                vk::CompareOp::eNever,
                0.f,
                0.f,
                vk::BorderColor::eFloatTransparentBlack,
                vk::False
            };

            auto device = _core->deviceManager().device();

            if (auto result = device.createSampler(info); result.result != vk::Result::eSuccess){
                throw std::runtime_error("Failed to create sampler");
            } else {
                _sampler = result.value;
            }
        }
};