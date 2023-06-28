#ifndef __RAINDROP_GRAPHICS_FACTORY_MODEL_FACTORY_HPP__
#define __RAINDROP_GRAPHICS_FACTORY_MODEL_FACTORY_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics::Factory{
	class ModelFactory : public Raindrop::Core::Asset::AssetFactory{
		public:
			ModelFactory(GraphicsContext& context);
			virtual ~ModelFactory() override;

			virtual std::shared_ptr<Core::Asset::Asset> createAsset(const std::filesystem::path& path) override;
			virtual void destroyAsset(std::shared_ptr<Core::Asset::Asset> asset) override;

			void registerExtensions(const std::shared_ptr<ModelFactory>& factory);

		private:
			GraphicsContext& _context;
			std::list<std::shared_ptr<Model>> _models;
	};
}

#endif