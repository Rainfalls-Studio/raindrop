#ifndef __RAINDROP_GRAPHICS_FACTORY_GRAPHICS_PIPELINE_FACTORY_HPP__
#define __RAINDROP_GRAPHICS_FACTORY_GRAPHICS_PIPELINE_FACTORY_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics::Factory{
	class GraphicsPipelineFactory : public Raindrop::Core::Asset::AssetFactory{
		public:
			GraphicsPipelineFactory(std::shared_ptr<Device> device, Core::Asset::AssetManager& assetManager, Core::Registry::Registry& registry, VkAllocationCallbacks* allocationCallbacks = nullptr);
			virtual ~GraphicsPipelineFactory() override;

			virtual std::shared_ptr<Core::Asset::Asset> createAsset(const std::filesystem::path& path) override;
			virtual void destroyAsset(std::shared_ptr<Core::Asset::Asset> asset) override;

			void registerExtensions(const std::shared_ptr<GraphicsPipelineFactory>& factory);
		
		private:
			class ExtensionToFormat;
			VkAllocationCallbacks* _allocationCallbacks = nullptr;
			Core::Asset::AssetManager& _assetManager;
			Core::Registry::Registry& _registry;
			std::shared_ptr<Device> _device;
			std::list<std::shared_ptr<GraphicsPipeline>> _pipelines;

			enum Format{
				NONE = 0,
				XML,
			};

			Format getFormat(const std::filesystem::path& path);
			Format getExtentionFormat(const std::wstring extension);

			std::shared_ptr<GraphicsPipeline> loadFromXML(const std::filesystem::path& path);

			std::list<std::shared_ptr<Shader>> getShadersXML(tinyxml2::XMLElement*  element);
			std::shared_ptr<Shader> getShaderXML(tinyxml2::XMLElement* shaderElement);

	};
}

#endif