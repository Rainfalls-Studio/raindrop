#pragma once

#include "Types.hpp"
#include <vector>
#include <stdexcept>
#include <filesystem>

namespace Raindrop::Filesystem{
    class Path{
        public:
            Path() = default;

            Path(const char* str) : _storage{str}{}
            Path(const String& str) : _storage{str}{}
            Path(const std::filesystem::path& path) : _storage{path}{}

            void clear(){
                _storage.clear();
            }

            bool empty() const{
                return _storage.empty();
            }

            String filename() const{
                return String(_storage.filename());
            }

            String extension() const{
                return String(_storage.extension());
            }

            Path parent() const {
                return String(_storage.parent_path());
            }

            bool isAbsolute() const{
                return _storage.is_absolute();
            }

            bool isRelative() const{
                return _storage.is_relative();
            }

            Size size() const{
                return _storage.native().size();
            }

            Path relativeTo(const Path& other) const{
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

            Path operator/(const Path& other){
                return Path(_storage / other._storage);
            }

        protected:
            std::filesystem::path _storage;
    };
}