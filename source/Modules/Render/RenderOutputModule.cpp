#include "Raindrop/Modules/Render/RenderOutputModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"

namespace Raindrop::Render{
    class RenderOutputErrorCategory : public std::error_category{
        public:
            const char* name() const noexcept override{
                return "Render Output";
            }

            std::string message(int ev) const override{
                using ErrorCode = RenderOutputModule::ErrorCode;

                switch (static_cast<ErrorCode>(ev)){
                    case ErrorCode::MISSING_SURFACE_SUPPORT: return "Missing surface support";
                    default: return "Unknown system error";
                }
            }
    };

    std::error_category& RenderOutputModule::error_category(){
        static RenderOutputErrorCategory category;
        return category;
    }


    RenderOutputModule::RenderOutputModule(){}
    RenderOutputModule::~RenderOutputModule(){}

    Modules::Result RenderOutputModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();
        _core = helper.getDependencyAs<RenderCoreModule>("RenderCore");

        return Modules::Result::Success();
    }

    void RenderOutputModule::shutdown(){
        shutdownAllOutputs();
        _outputs.clear();
    }

    RenderOutputModule::Name RenderOutputModule::name() const noexcept{
        return "RenderOutput";
    }

    Modules::DependencyList RenderOutputModule::dependencies() const noexcept{
        return {
            Modules::Dependency("RenderCore")
        };
    }

    Modules::Result RenderOutputModule::dependencyReload(const Name& name){
        if (name == "RenderCore"){
            _core = _engine->getModuleManager().getModuleAs<RenderCoreModule>("RenderCore");
            rebuildAllOutputs();
        }
        return Modules::Result::Success();
    }

    Modules::Result RenderOutputModule::dependencyShutdown(const Name&){
        return Modules::Result::Error();
    }

    void RenderOutputModule::initializeAllOutputs(){
        for (auto [it, output] : _outputs){
            output->initialize(*_core);
        }
    }

    void RenderOutputModule::shutdownAllOutputs(){
        for (auto [it, output] : _outputs){
            output->shutdown();
        }
    }

    void RenderOutputModule::rebuildAllOutputs(){
        for (auto [it, output] : _outputs){
            output->shutdown();
            output->initialize(*_core);
        }
    }

    SharedRenderOutput RenderOutputModule::getOutput(const IRenderOutput::Name& name){
        auto it = _outputs.find(name);
        if (it == _outputs.end()) return nullptr;
        return it->second;
    }

    void RenderOutputModule::registerOutput(const IRenderOutput::Name& name, SharedRenderOutput output){
        _outputs[name] = output;
        output->initialize(*_core);
    }

    void RenderOutputModule::unregisterOutput(const IRenderOutput::Name& name){
        _outputs.erase(name);
    }
}