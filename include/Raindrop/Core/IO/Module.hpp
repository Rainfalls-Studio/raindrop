#ifndef __RAINDROP_CORE_IO_MODULE_HPP__
#define __RAINDROP_CORE_IO_MODULE_HPP__

#include <Core/IO/common.hpp>

namespace Raindrop::Core::IO{
	class RAINDROP_API Module{
		public:
			Module(const std::filesystem::path& path);
			~Module();

			bool isOpen() const;
			const std::filesystem::path& filepath() const;
			void* getFnc(const char* name) const;

			template<typename T, typename... Args>
			std::shared_ptr<T> loadPlugin(const char* name, Args&&... args){
				using FncType = std::shared_ptr<T>(*)(Args...);
				std::string fncName = "create_" + std::string(name);

				FncType fnc = nullptr;
				
				try{
					fnc = (FncType)_dll.getProc(fncName.c_str());
				} catch (const std::exception &e){
					throw e;
				}

				return fnc(args...);
			}
		
		private:
			DLLReader _dll;
			bool _loaded;

			bool init();
			void shutdown();
	};
}

#endif