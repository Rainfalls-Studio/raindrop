#pragma once

#include <string_view>

namespace Raindrop::Utils{
    template <typename T>
    constexpr std::string_view type_name() {
        #if defined(__clang__) || defined(__GNUC__)
            constexpr std::string_view p = __PRETTY_FUNCTION__;
            constexpr std::string_view prefix = "std::string_view Raindrop::Core::Utils::type_name() [with T = ";
            constexpr std::string_view suffix = "]";
            return p.substr(prefix.size(), p.size() - prefix.size() - suffix.size());
        #elif defined(_MSC_VER)
            constexpr std::string_view p = __FUNCSIG__;
            constexpr std::string_view prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Raindrop::Core::Utils::type_name<";
            constexpr std::string_view suffix = ">(void)";
            return p.substr(prefix.size(), p.size() - prefix.size() - suffix.size());
        #else
            return "unknown";
        #endif
    }
}