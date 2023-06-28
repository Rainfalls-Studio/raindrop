#include <Raindrop/Graphics/factory/ModelFactory.hpp>
#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Core/Asset/AssetManager.hpp>

namespace Raindrop::Graphics::Factory{
	ModelFactory::ModelFactory(GraphicsContext& context) : _context{context}{

	}

	ModelFactory::~ModelFactory(){
		_models.clear();
	}

	std::shared_ptr<Core::Asset::Asset> ModelFactory::createAsset(const std::filesystem::path& path){
		Builders::ModelBuilder builder(path);
		std::shared_ptr<Model> model = builder.build(_context);

		_models.push_back(model);
		return std::static_pointer_cast<Core::Asset::Asset>(model);
	}

	void ModelFactory::destroyAsset(std::shared_ptr<Core::Asset::Asset> asset){
		_models.remove(std::static_pointer_cast<Model>(asset));
	}

	void ModelFactory::registerExtensions(const std::shared_ptr<ModelFactory>& factory){
		auto& assetManager = _context->assetManager;

		assetManager.linkFactory(".3mf", factory);
		assetManager.linkFactory(".dae", factory);
		assetManager.linkFactory(".blend", factory);
		assetManager.linkFactory(".bvh", factory);
		assetManager.linkFactory(".3ds", factory);
		assetManager.linkFactory(".ase", factory);
		assetManager.linkFactory(".gITF", factory);
		assetManager.linkFactory(".fbx", factory);
		assetManager.linkFactory(".ply", factory);
		assetManager.linkFactory(".dxf", factory);
		assetManager.linkFactory(".ifc", factory);
		assetManager.linkFactory(".iqm", factory);
		assetManager.linkFactory(".nff", factory);
		assetManager.linkFactory(".vta", factory);
		assetManager.linkFactory(".smd", factory);
		assetManager.linkFactory(".mdl", factory);
		assetManager.linkFactory(".mdl2", factory);
		assetManager.linkFactory(".mdl3", factory);
		assetManager.linkFactory(".pk3", factory);
		assetManager.linkFactory(".mdc", factory);
		assetManager.linkFactory(".md5mesh", factory);
		assetManager.linkFactory(".md5anim", factory);
		assetManager.linkFactory(".md5camera", factory);
		assetManager.linkFactory(".x", factory);
		assetManager.linkFactory(".q3o", factory);
		assetManager.linkFactory(".q3s", factory);
		assetManager.linkFactory(".raw", factory);
		assetManager.linkFactory(".ac", factory);
		assetManager.linkFactory(".ac3d", factory);
		assetManager.linkFactory(".stl", factory);
		assetManager.linkFactory(".irrmesh", factory);
		assetManager.linkFactory(".irr", factory);
		assetManager.linkFactory(".off", factory);
		assetManager.linkFactory(".obj", factory);
		assetManager.linkFactory(".ter", factory);
		assetManager.linkFactory(".mdl", factory);
		assetManager.linkFactory(".hmp", factory);
		assetManager.linkFactory(".mesh.xml", factory);
		assetManager.linkFactory(".skeleton.xml", factory);
		assetManager.linkFactory(".material.xml", factory);
		assetManager.linkFactory(".ogex", factory);
		assetManager.linkFactory(".ms3d", factory);
		assetManager.linkFactory(".lwo", factory);
		assetManager.linkFactory(".lws", factory);
		assetManager.linkFactory(".lxo", factory);
		assetManager.linkFactory(".csm", factory);
		assetManager.linkFactory(".cob", factory);
		assetManager.linkFactory(".scn", factory);
		assetManager.linkFactory(".xgl", factory);
	}

}