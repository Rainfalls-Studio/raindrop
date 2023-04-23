#include <Managers/AssetManager.hpp>
#include <Core/Filesystem/Filesystem.hpp>

namespace Raindrop::Managers{
	AssetManager::AssetManager(Raindrop::Core::Memory::Allocator& allocator) : _allocator{allocator}{}
	AssetManager::~AssetManager(){}

	AssetPtr AssetManager::_getOrCreate(Core::Memory::Allocator& allocator, const char* path){
		if (exists(path)){
			return *_pathToAssetsMap.get(path);
		}

		AssetFactory* factory = _getFactoryFromPath(path);
		AssetPtr asset = factory->create(allocator, path);

		_pathToAssetsMap.insert(path, asset);
		return asset;
	}

	AssetFactory* AssetManager::_getFactory(const char* extension) const{
		AssetFactory* factory = _extensionToFactoryMap.lookup(extension, nullptr);
		if (!factory) throw std::invalid_argument("invalid file extension, cannot find a suitable factory");
		return factory;
	}

	bool AssetManager::exists(const char* path){
		return _pathToAssetsMap.has(path);
	}

	AssetFactory* AssetManager::_getFactoryFromPath(const char* extension) const{
		Core::Filesystem::Path path(_allocator, extension);
		Core::Filesystem::Extention ext = Core::Filesystem::getExtention(path, _allocator);
		return _getFactory(ext.str());
	}
}