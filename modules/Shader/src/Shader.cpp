#include "Raindrop/Modules/Render/Shaders/Shader.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Render{

    class ShaderErrorCategory : public std::error_category{
        public:
            const char* name() const noexcept override{
                return "Shader";
            }

            std::string message(int ev) const override{
                using ErrorCode = Shader::ErrorCode;

                switch (static_cast<ErrorCode>(ev)){
                    case ErrorCode::FAILED_SHADER_MODULE_CREATION: return "Failed to create vulkan shader module";
                    default: return "Unknown system error";
                }
            }
    };

    std::error_category& Shader::error_category(){
        static ShaderErrorCategory category;
        return category;
    }

    Shader::Shader(
        std::shared_ptr<Filesystem::FilesystemModule> filesystem,
        std::shared_ptr<RenderCoreModule> core,
        const Filesystem::Path& path,
        const std::string& entryPoint
    ) : 
        _filesystem(filesystem),
        _core{core},
        _path{path},
        _entryPoint{entryPoint}
    {}

    Shader::~Shader(){
        if (auto result = unload(); !result){
            spdlog::warn("Failed to unload shader \"{}\"", _path);
        }
    }

    std::expected<void, Error> Shader::load(){

        // skip if already loaded
        if (_module) return {};

        // load code
        if (auto result = getCodeImpl(); !result){
            return std::unexpected(result.error());
        } else {
            _code = result.value();
        }
        
        // create module
        if (auto result = createModule(); !result){
            return std::unexpected(result.error());
        } else {
            _module = result.value();
        }

        return {};
    }

    std::expected<void, Error> Shader::reload(){
        return unload()
            .and_then([&]{return load();});
    }

    std::expected<void, Error> Shader::unload(){
        auto device = _core->deviceManager().device();

        if (_module){
            device.destroyShaderModule(_module);
            _module = VK_NULL_HANDLE;
        }

        return {};
    }

    bool Shader::isLoaded(){
        return _module;
    }

    uint32_t Shader::getMemoryUsage() const{
        return static_cast<uint32_t>(_code.size() * sizeof(uint32_t));
    }

    vk::ShaderStageFlagBits Shader::stage() const{
        return _stage;
    }

    vk::ShaderModule Shader::module() const{
        return _module;
    }

    std::expected<vk::ShaderModule, Error> Shader::createModule(){
        vk::ShaderModuleCreateInfo info{
            {},
            static_cast<uint32_t>(_code.size()) * sizeof(uint32_t),
            _code.data()
        };

        auto device = _core->deviceManager().device();

        if (auto result = device.createShaderModule(info); result.result == vk::Result::eSuccess){
            return result.value;
        } else {
            return std::unexpected(Error(FailedShaderModuleCreationError(), vk::to_string(result.result)));
        }
    }

    vk::PipelineShaderStageCreateInfo Shader::stageInfo() const{
        return vk::PipelineShaderStageCreateInfo{
            {},
            _stage,
            _module,
            _entryPoint.c_str(),
            {}
        };
    }
}