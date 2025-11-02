#pragma once

#include "Raindrop/Modules/Filesystem/FilesystemModule.hpp"
#include "Raindrop/Modules/Render/Core/RenderCoreModule.hpp"
#include "Raindrop/Modules/Asset/Factory.hpp"
#include "Shader.hpp"

namespace Raindrop::Render{
    class ShaderFactory : public Asset::Factory{
        public:
            ShaderFactory(Engine& engine);
			virtual ~ShaderFactory();
			
			// get or create an asset based of the given parameters
			// NOTE : this function should NOT load assets but only create a valid class to reference them
			virtual std::shared_ptr<Asset::Asset> getOrCreate(const Filesystem::Path& path) override;

			virtual std::string getName() const noexcept override;
        private:
            std::shared_ptr<Filesystem::FilesystemModule> _filesystem;
            std::shared_ptr<RenderCoreModule> _renderCore;

            std::unordered_map<Filesystem::Path, std::weak_ptr<Shader>> _shaders;
    };
}