#pragma once

#include <Raindrop/Engine.hpp>
#include <Raindrop/Modules/IModule.hpp>
#include <RenderCore/RenderCoreModule.hpp>

#include "IRenderOutput.hpp"

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

            virtual Modules::Name name() const noexcept override;
            virtual Modules::DependencyList dependencies() const noexcept override;

            virtual Modules::Result dependencyReload(const Modules::Name& name) override;
            virtual Modules::Result dependencyShutdown(const Modules::Name& name) override;

            SharedRenderOutput getOutput(const IRenderOutput::Name& name);

            void setMainOutput(const IRenderOutput::Name& name);

            template<typename T>
            std::shared_ptr<T> getOutputAs(const IRenderOutput::Name& name){
                static_assert(std::is_base_of<IRenderOutput, T>::value, "The type T must be derived from IRenderOutput");
                return std::dynamic_pointer_cast<T>(getOutput(name));
            }

            void createOutput(const IRenderOutput::Name& name, SharedRenderOutput output);
            void unregisterOutput(const IRenderOutput::Name& name);

            template<typename T>
            std::shared_ptr<T> createOutput(const IRenderOutput::Name& name){
                static_assert(std::is_base_of<IRenderOutput, T>::value, "The type T must be derived from IRenderOutput");
                std::shared_ptr<T> output = std::make_shared<T>();
                createOutput(name, std::static_pointer_cast<IRenderOutput>(output));
                return output;
            }

            template<typename T, typename... Args>
            std::shared_ptr<T> createOutput(const IRenderOutput::Name& name, Args&&... args){
                static_assert(std::is_base_of<IRenderOutput, T>::value, "The type T must be derived from IRenderOutput");
                auto output = std::make_shared<T>(std::forward<Args>(args)...);
                createOutput(name, std::static_pointer_cast<IRenderOutput>(output));
                return output;
            }
        
        private:
            struct RenderOutputInfo{
                SharedRenderOutput output;
                IRenderOutput::Name name;
                bool available = false;
                bool enabled = true;

                inline bool valid() const noexcept{
                    return output != nullptr && enabled;
                }
            };

            Engine* _engine;
            std::shared_ptr<RenderCoreModule> _core;
            std::unordered_map<IRenderOutput::Name, std::shared_ptr<RenderOutputInfo>> _outputs;
            
            // The output that will allow pacing. If invalid, it will select the first registred output
            IRenderOutput::Name _mainOutputName = "main";
            std::shared_ptr<RenderOutputInfo> _mainOutput;

            void findMainOutput();
            std::shared_ptr<RenderOutputInfo> findFirstValid();

            void initializeAllOutputs();
            void shutdownAllOutputs();
            void rebuildAllOutputs();

            enum class RenderOutputResult{
                SUCCESS,
                SKIP,
                ERROR
            };
    };
}