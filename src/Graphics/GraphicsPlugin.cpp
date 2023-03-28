#include "GraphicsPlugin.hpp"

namespace Raindrop::Graphics{
	Plugin* Plugin::create(Core::Memory::Allocator& allocator, Core::IO::Module& module, Context& context, Core::Scene::Scene& scene, const char* name){
		using LoadModuleFnc = Plugin*(*)(Core::Memory::Allocator&, Context&, Core::Scene::Scene&);
		LoadModuleFnc loadModule = (LoadModuleFnc)module.getFnc(name);

		if (loadModule){
			RAINDROP_log(ERROR, GRAPHICS, "cannot found %s graphics plugin in %s module", name, module.filepath());
			return nullptr;
		}

		return loadModule(allocator, context, scene);
	}
}