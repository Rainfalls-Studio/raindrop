#pragma once

#include <Asset/Asset.hpp>
#include <RenderCore/RenderCoreModule.hpp>

#include <Filesystem/FilesystemModule.hpp>
#include <vulkan/vulkan.hpp>

namespace Raindrop::Render{
    class Shader : public Asset::Asset{
        public:
            enum class ErrorCode{
                FAILED_SHADER_MODULE_CREATION
            };

            static std::error_category& error_category();

            static inline std::error_code make_error_code(ErrorCode e){
                return {static_cast<int>(e), error_category()};
            }

            static inline std::error_code FailedShaderModuleCreationError() {return make_error_code(ErrorCode::FAILED_SHADER_MODULE_CREATION);}

            Shader(
                std::shared_ptr<Filesystem::FilesystemModule> filesystem,
                std::shared_ptr<RenderCoreModule> core,
                const Filesystem::Path& path,
                const std::string& entryPoint = "main"
            );

            ~Shader();

            virtual std::expected<void, Error> load() override;
            virtual std::expected<void, Error> reload() override;
            virtual std::expected<void, Error> unload() override;
            virtual bool isLoaded() override;

            virtual std::uint32_t getMemoryUsage() const override;

            vk::ShaderStageFlagBits stage() const;
            vk::ShaderModule module() const;
            vk::PipelineShaderStageCreateInfo stageInfo() const;
        
        protected:
            virtual std::expected<std::vector<uint32_t>, Error> getCodeImpl() = 0;

            std::shared_ptr<Filesystem::FilesystemModule> _filesystem;
            std::shared_ptr<Raindrop::Render::RenderCoreModule> _core;

            Filesystem::Path _path;
            std::string _entryPoint;
            std::vector<uint32_t> _code;
            vk::ShaderStageFlagBits _stage;
            vk::ShaderModule _module;

            std::expected<vk::ShaderModule, Error> createModule();
    };
}