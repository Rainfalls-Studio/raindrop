#include <Wrappers/ModuleWrapper.hpp>

namespace Raindrop::Wrappers{
	ModuleWrapper::ModuleWrapper(){}
	ModuleWrapper::ModuleWrapper(std::weak_ptr<Module> module) : _module{module}{}
	ModuleWrapper::~ModuleWrapper(){}

	const std::filesystem::path& ModuleWrapper::filepath() const{
		if (auto ptr = _module.lock()){
			return ptr->filepath();
		} else {
			return "";
		}
	}

	void* ModuleWrapper::getFnc(const char* name) const{
		if (auto ptr = _module.lock()){
			return ptr->getFnc(name);
		} else {
			return nullptr;
		}
	}

	bool ModuleWrapper::exprired() const{
		return _module.expired();
	}
}