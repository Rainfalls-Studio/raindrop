#pragma once

#include "Shader.hpp"
#include <Filesystem/FilesystemModule.hpp>

namespace Shader{
    class GLSLShader : public Shader{
        public:
            GLSLShader(
                std::shared_ptr<Filesystem::FilesystemModule> filesystem,
                std::shared_ptr<Render::RenderCoreModule> core,
                const Filesystem::Path& path,
                const std::string& entryPoint = "main"
            );

            virtual ~GLSLShader() override = default;

        protected:
            virtual std::expected<std::vector<uint32_t>, Raindrop::Error> getCodeImpl() override;
    };
}