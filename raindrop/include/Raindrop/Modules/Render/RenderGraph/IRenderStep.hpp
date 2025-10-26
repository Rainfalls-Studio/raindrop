#pragma once

#include <vulkan/vulkan.hpp>
#include <memory>

namespace Raindrop::Render{
    class IRenderStep{
        public:
            virtual ~IRenderStep() = default;

            virtual void initialize() {}
            virtual void record(vk::CommandBuffer commandBuffer) = 0;
    };

    template<typename T>
    inline std::unique_ptr<IRenderStep> MakeStep(){
        static_assert(std::is_base_of_v<IRenderStep, T>, "T must derive from IRenderStep");
        return std::make_unique<T>();
    }

    template<typename T, typename... Args>
    inline std::unique_ptr<IRenderStep> MakeStep(Args&&... args){
        static_assert(std::is_base_of_v<IRenderStep, T>, "T must derive from IRenderStep");
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename... Steps>
    inline std::vector<std::unique_ptr<IRenderStep>> MakeSteps(Steps&&... steps) {
        std::vector<std::unique_ptr<IRenderStep>> v;
        (v.push_back(std::move(steps)), ...);
        return v;
    }
}