#pragma once

#include "Types.hpp"
#include <filesystem>
#include <format>
#include <fmt/format.h>

namespace Filesystem{
    class Path{
        public:
            Path() = default;

            Path(const char* str) : _storage{str}{}
            Path(const String& str) : _storage{str}{}
            Path(const std::filesystem::path& path) : _storage{path}{}

            inline void clear(){
                _storage.clear();
            }

            inline bool empty() const{
                return _storage.empty();
            }

            inline String filename() const{
                return String(_storage.filename());
            }

            inline String extension() const{
                return String(_storage.extension());
            }

            inline Path parent() const {
                return Path(_storage.parent_path());
            }

            inline bool isAbsolute() const{
                return _storage.is_absolute();
            }

            inline bool isRelative() const{
                return _storage.is_relative();
            }

            inline Size size() const{
                return _storage.native().size();
            }

            inline Path relativeTo(const Path& other) const{
                return Path(_storage.lexically_relative(other._storage));
            }

            bool isPrefixOf(const Path& other) const{
                auto normA = std::filesystem::weakly_canonical(_storage);
                auto normB = std::filesystem::weakly_canonical(other._storage);

                auto itA = normA.begin();
                auto itB = normB.begin();

                for (; itA != normA.end(); ++itA, ++itB) {
                    if (itB == normB.end() || *itA != *itB)
                        return false;
                }

                return true;
            }

            decltype(auto) str() { return _storage.string(); }
            decltype(auto) str() const { return _storage.string(); }

            inline Path operator/(const Path& other){
                return Path(_storage / other._storage);
            }

            inline bool operator==(const Path& other) const{
                return _storage == other._storage;
            }

            // inline std::size_t hash() const noexcept{
            // }

            friend class std::hash<Path>;
            friend class std::formatter<Path>; // std::format
            friend class fmt::formatter<Path>; // spdlog

        protected:
            std::filesystem::path _storage;
    };
}

namespace std{
    template<>
    struct hash<Filesystem::Path>{
        std::size_t operator()(const Filesystem::Path& path) const noexcept{
            static std::hash<std::filesystem::path> h; 
            return h(path._storage);
        }
    };

    template <>
    struct formatter<Filesystem::Path> : formatter<std::string>{
        auto format(const Filesystem::Path& path, std::format_context& ctx) const {
            return formatter<std::string>::format(path._storage.string(), ctx);
        }
    };
}

template <>
struct fmt::formatter<Filesystem::Path> : fmt::formatter<std::string> {
    auto format(const Filesystem::Path& path, fmt::format_context& ctx) const {
        return fmt::formatter<std::string>::format(path._storage.string(), ctx);
    }
};