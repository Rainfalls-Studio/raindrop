#ifndef __RAINDROP_MANAGERS_MODULE_MANAGER_HPP__
#define __RAINDROP_MANAGERS_MODULE_MANAGER_HPP__

#include <Managers/common.hpp>

namespace Raindrop::Managers{
	class ModuleManager{
		public:
			ModuleManager() = default;
			~ModuleManager() = default;

			template<typename T = Module>
			std::weak_ptr<T> getOrLoad(const std::filesystem::path& path){
				{
					auto it = _pathToModuleMap.find(path);
					if (it != _pathToModuleMap.end()) return std::dynamic_pointer_cast<T>(it->second);
				}

				RAINDROP_log(INFO, IO, "loading new module ...");
				std::shared_ptr<Module> module = std::make_shared<T>(path);
				_pathToModuleMap[path] = module;
				RAINDROP_log(INFO, IO, "module loaded with success !");

				return std::dynamic_pointer_cast<T>(module);
			}

			void unload(const std::filesystem::path& path){
				auto it = _pathToModuleMap.find(path);

				if (it != _pathToModuleMap.end()){
					_pathToModuleMap.erase(it);	
				} else {
					std::stringstream err;
					err << "failed to unload " << path << ", the module is not loaded";
					throw std::runtime_error(err.str());
				}
			}

			template<typename T=Plugin, typename... Args>
			std::shared_ptr<T> createPlugin(const char* name, Args&&... args){
				RAINDROP_log(INFO, IO, "loading new plugin : %s", name);
				using FncType = std::shared_ptr<T>(*)(Args...);
				FncType fnc = reinterpret_cast<FncType>(findConstructor(name));

				if (!fnc){
					RAINDROP_log(ERROR, IO, "failed to load plugin");
					std::stringstream err;
					err << "cannot find given plugin in the loaded modules : " << name;
					throw std::runtime_error(err.str());
				}

				RAINDROP_log(INFO, IO, "plugin constructor found with success !");
				return fnc(args...);
			}

			bool exists(const std::filesystem::path& path){
				auto it = _pathToModuleMap.find(path);
				return it != _pathToModuleMap.end();
			}

		private:
			std::unordered_map<std::filesystem::path, std::shared_ptr<Module>> _pathToModuleMap;

			void* findConstructor(const char* name){
				std::string constructor = std::string("create_") + name;
				void* fnc = nullptr;
				
				for (const auto it : _pathToModuleMap){
					try{
						fnc = it.second->getFnc(constructor.c_str());
						break;
					} catch (const std::exception &){
						continue;
					}
				}
				return fnc;
			}
	};
}

#endif