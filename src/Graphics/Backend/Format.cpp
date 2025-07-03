#include "Raindrop/Graphics/Backend/Format.hpp"
#include <utility>

namespace Raindrop::Graphics::Backend{
    Format::Format(FormatType format) : _format{format}{}
    Format::Format(const Format& other) : _format{other._format}{}

    Format::Format(Format&& other){
        std::swap(other._format, _format);
    }

    Format& Format::operator=(FormatType format){
        _format = format;
        return *this;
    }

    Format& Format::operator=(const Format& other){
        _format = other._format;
        return *this;
    }

    Format& Format::operator=(Format&& other){
        std::swap(_format, other._format);
        return *this;
    }

    Format::operator FormatType() const noexcept{
        return _format;
    }
}