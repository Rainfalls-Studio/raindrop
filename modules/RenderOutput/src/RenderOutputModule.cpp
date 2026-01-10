#include <RenderOutput/RenderOutputModule.hpp>
#include <Raindrop/Modules/InitHelper.hpp>

#include <spdlog/spdlog.h>

extern "C" RAINDROP_EXPORT Raindrop::IModule* CreateModule(){
	return new Render::RenderOutputModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::IModule* module){
	delete module;
}


namespace Render{
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

    Raindrop::Result RenderOutputModule::initialize(Raindrop::InitHelper& helper){
        _engine = &helper.engine();
        _logger = helper.logger();
        _core = helper.getDependencyAs<RenderCoreModule>("RenderCore");

        return Raindrop::Result::Success();
    }

    void RenderOutputModule::shutdown(){
        shutdownAllOutputs();
        _outputs.clear();
    }

    void RenderOutputModule::initializeAllOutputs(){
        for (auto [it, info] : _outputs){
            _logger->info("Initializing render output \"{}\"", it);

            if (auto result = info->output->initialize(*_engine); !result){
                auto& error = result.error();
                _logger->error("Failed to initialize render output \"{}\" : {}, {}", it, error.message(), error.reason());
            }
        }
    }

    void RenderOutputModule::shutdownAllOutputs(){
        for (auto [it, info] : _outputs){
            _logger->info("Shuting down render output \"{}\"", it);
            info->output->shutdown();
        }
    }

    void RenderOutputModule::rebuildAllOutputs(){
        for (auto [it, info] : _outputs){
            _logger->info("Rebuilding render output \"{}\"", it);
            info->output->shutdown();

            if (auto result = info->output->initialize(*_engine); !result){
                auto& error = result.error();
                _logger->error("Failed to initialize render output \"{}\" : {}, {}", it, error.message(), error.reason());
            }
        }
    }

    SharedRenderOutput RenderOutputModule::getOutput(const IRenderOutput::Name& name){
        auto it = _outputs.find(name);
        if (it == _outputs.end()) return nullptr;

        return it->second->output;
    }

    void RenderOutputModule::createOutput(const IRenderOutput::Name& name, SharedRenderOutput output){
        _logger->info("Registering and initializing render output \"{}\"", name);
        
        if (_outputs.count(name) > 0){
            _logger->info("Overwriting existing output \"{}\"", name);
        }
        _outputs[name] = std::make_shared<RenderOutputInfo>(output, name);

        if (auto result = output->initialize(*_engine); !result){
            auto& error = result.error();
            _logger->error("Failed to initialize render output \"{}\" : {}, {}", name, error.message(), error.reason());
        }

        findMainOutput();
    }

    void RenderOutputModule::unregisterOutput(const IRenderOutput::Name& name){
        _outputs.erase(name);
        findMainOutput();
    }

    void RenderOutputModule::setMainOutput(const IRenderOutput::Name& name){
        _mainOutputName = name;
        findMainOutput();
    }

    void RenderOutputModule::findMainOutput(){
        if (_outputs.empty()){
            _logger->error("There are no registred render output !");
            _mainOutput = nullptr;
            return;
        }

        auto it = _outputs.find(_mainOutputName);
        if (it == _outputs.end()){
            _mainOutput = findFirstValid();

            if (_mainOutput)
                _logger->error("No render outputs match's the main output's name (\"{}\"). Falling back to \"{}\"", _mainOutputName, _mainOutput->name);

            return;
        }

        _mainOutput = it->second;
        if (!_mainOutput && !_mainOutput->valid()){
            _mainOutput = findFirstValid();

            if (_mainOutput)
                _logger->error("Found the main output, but it is not valid. Falling back to \"{}\"", _mainOutput->name);
        }
    }

    std::shared_ptr<RenderOutputModule::RenderOutputInfo> RenderOutputModule::findFirstValid(){
        for (auto [it, info] : _outputs){
            if (info && info->valid()){
                return info;
            }
        }
        _logger->error("There are no valid outputs registred !");
        return nullptr;
    }
}