#ifndef __RAINDROP_GRAPHICS_FACTORY_SHADER_FACTORY_HPP__
#define __RAINDROP_GRAPHICS_FACTORY_SHADER_FACTORY_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics::Factory{
	class ShaderFactory : public Raindrop::Core::Asset::AssetFactory{
		public:
			ShaderFactory(std::shared_ptr<Device> device, VkAllocationCallbacks* allocationCallbacks = nullptr);
			virtual ~ShaderFactory() override;

			virtual std::shared_ptr<Core::Asset::Asset> createAsset(const std::filesystem::path& path) override;
			virtual void destroyAsset(std::shared_ptr<Core::Asset::Asset> asset) override;

		private:
			VkAllocationCallbacks* _allocationCallbacks = nullptr;
			std::shared_ptr<Device> _device;

			std::shared_ptr<Shader> loadSPV(const std::filesystem::path& path);

			std::list<std::shared_ptr<Shader>> _shaders;
	};
}

#endif