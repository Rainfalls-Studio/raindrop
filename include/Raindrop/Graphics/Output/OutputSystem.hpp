#pragma once

#include "Raindrop/Graphics/System/IRenderSystem.hpp"
#include "IOutputTarget.hpp"
#include <memory>

namespace Raindrop::Graphics::Output{
    class OutputSystem : public System::IRenderSystem{
        public:
            virtual ~OutputSystem() = default;

            virtual void initialize(GraphicsEngine& engine) override;
            virtual void shutdown() override;

            virtual const char* name() const override;

            std::shared_ptr<IOutputTarget> getOutput();
            void setOutput(const std::shared_ptr<IOutputTarget>& output);

            template<typename T>
            void setOutput(const std::shared_ptr<T>& output){
                setOutput(std::static_pointer_cast<IOutputTarget>(output));
            }

        private:
            GraphicsEngine* _engine;
            std::weak_ptr<IOutputTarget> _output;
    };
}