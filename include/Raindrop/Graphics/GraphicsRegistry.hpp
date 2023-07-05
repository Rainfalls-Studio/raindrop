#ifndef __RAINDROP_GRAPHICS_GRAPHICS_REGISTRY_HPP__
#define __RAINDROP_GRAPHICS_GRAPHICS_REGISTRY_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class GraphicsRegistry{
		public:
			GraphicsRegistry() = default;
			~GraphicsRegistry() = default;

			void*& get(const std::string& ID){
				auto it = _map.find(ID);
				if (it == _map.end()){
					throw std::runtime_error("invalid ID");
				}
				return _map[ID];
			}

			const void* get(const std::string& ID) const{
				auto it = _map.find(ID);
				if (it == _map.end()){
					throw std::runtime_error("invalid ID");
				}
				return it->second;
			}

			void*& operator[](const std::string& ID){
				return _map[ID];
			}

		private:
			std::unordered_map<std::string, void*> _map;
	};
}

#endif