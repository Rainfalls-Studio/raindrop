#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "IRenderOutput.hpp"
#include "RenderCoreModule.hpp"
#include "Raindrop/Engine.hpp"

namespace Raindrop::Render{
    class RenderOutputModule : public Modules::IModule{
        public:
            enum class ErrorCode{
                FAILED_OBJECT_CREATION,
                MISSING_SURFACE_SUPPORT,
            };

            static std::error_category& error_category();

            static inline std::error_code make_error_code(ErrorCode e){
                return {static_cast<int>(e), error_category()};
            }

            static inline std::error_code FailedObjectCreationError() {return make_error_code(ErrorCode::FAILED_OBJECT_CREATION);}
            static inline std::error_code MissingSurfaceSupportError() {return make_error_code(ErrorCode::MISSING_SURFACE_SUPPORT);}

            RenderOutputModule();
            ~RenderOutputModule();

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            virtual void shutdown() override;

            virtual Name name() const noexcept override;
            virtual Modules::DependencyList dependencies() const noexcept override;

            virtual Modules::Result dependencyReload(const Name& name) override;
            virtual Modules::Result dependencyShutdown(const Name& name) override;

            SharedRenderOutput getOutput(const IRenderOutput::Name& name);

            template<typename T>
            std::shared_ptr<T> getOutputAs(const IRenderOutput::Name& name){
                static_assert(std::is_base_of<IRenderOutput, T>::value, "The type T must be derived from IRenderOutput");
                return std::dynamic_pointer_cast<T>(getOutput(name));
            }

            void registerOutput(const IRenderOutput::Name& name, SharedRenderOutput output);
            void unregisterOutput(const IRenderOutput::Name& name);

            template<typename T>
            void registerOutput(const IRenderOutput::Name& name){
                static_assert(std::is_base_of<IRenderOutput, T>::value, "The type T must be derived from IRenderOutput");
                registerOutput(name, std::static_pointer_cast<IRenderOutput>(std::make_shared<T>()));
            }

            template<typename T, typename... Args>
            void registerOutput(const IRenderOutput::Name& name, Args&&... args){
                static_assert(std::is_base_of<IRenderOutput, T>::value, "The type T must be derived from IRenderOutput");
                registerOutput(name, std::static_pointer_cast<IRenderOutput>(std::make_shared<T>(std::forward<Args>(args)...)));
            }
        
        private:
            Engine* _engine;
            std::shared_ptr<RenderCoreModule> _core;
            RenderOutputMap _outputs;

            void initializeAllOutputs();
            void shutdownAllOutputs();
            void rebuildAllOutputs();
    };
}