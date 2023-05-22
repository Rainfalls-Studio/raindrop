#ifndef __RAINDROP_GRAPHICS_EXCEPTION_HPP__
#define __RAINDROP_GRAPHICS_EXCEPTION_HPP__

#include <stdexcept>

namespace Raindrop::Graphics{
	class MissingLayer : public ::std::exception{
		public:
			MissingLayer(std::string layer) : _layer(layer) {}

			virtual const char* what() const throw() {
				return _layer.c_str();
			}

		private:
			std::string _layer;
	};

	class MissingExtension : public ::std::exception{
		public:
			MissingExtension(std::string extension) : _extension(extension) {}

			virtual const char* what() const throw() {
				return _extension.c_str();
			}

		private:
			std::string _extension;
	};
}

#endif