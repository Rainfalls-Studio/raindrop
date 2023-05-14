#ifndef __RAINDROP_WRAPPERS_MODULE_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_MODULE_WRAPPER_HPP__

#include <Wrappers/common.hpp>

namespace Raindrop::Wrappers{
	class ModuleWrapper{
		public:
			ModuleWrapper(std::weak_ptr<Module> module);
			ModuleWrapper();
			~ModuleWrapper();

			const std::filesystem::path& filepath() const;

			template<typename T, typename... Args>
			std::function<T(Args...)> getFnc(const char* name){
				using FncType = T(*)(Args...);
				std::function<T(Args...)> fnc = reinterpret_cast<FncType>(getFnc(name));
				return fnc;
			}

			bool exprired() const;

		private:
			std::weak_ptr<Module> _module;

			void* getFnc(const char* name) const;
	};
}

#endif