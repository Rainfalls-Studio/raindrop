#pragma once

#include "Types.hpp"
#include <vector>
#include <stdexcept>

namespace Raindrop::Filesystem{
    template<typename Derived>
    class PathBase{
        public:
            void clear(){
                getSelf().str().clear();
            }

            bool empty() const{
                return getSelf().str().empty();
            }

            StringView filename() const{
                auto& s = getSelf().str();
                size_t pos = s.find_last_of('/');
                return pos == String::npos ? s : s.substr(pos + 1);
            }

            StringView extension() const{
                auto& s = getSelf().str();
                size_t pos = s.find_last_of('.');
                return pos == String::npos ? s : s.substr(pos + 1);
            }

            PathBase<Derived> parent(){
                auto& s = getSelf().str();
                size_t pos = s.find_last_of('/');
                return pos == String::npos ? *this : PathBase<Derived>(s.substr(0, pos));
            }

            PathBase<Derived> isAbsolute() const{
                auto& s = getSelf().str();
                return s[0] == '/';
            }

            PathBase<Derived> isRelative() const{
                auto& s = getSelf().str();
                return !isAbsolute() && !s.empty() && s[0] != '/';
            }

            PathBase<Derived> isRoot() const{
                auto& s = getSelf().str();
                return s == "/";
            }

            Size size() const{
                return getSelf().str().size();
            }

            bool isPrefixOf(const PathBase& other) const {
                const std::string& prefix = getSelf().str();
                const std::string& target = other.getSelf().str();

                if (target.size() < prefix.size()) return false;
                if (target.compare(0, prefix.size(), prefix) != 0) return false;

                // Ensure prefix matches whole component (avoid "{sources}" matching "{sourcesExtra}")
                if (target.size() == prefix.size()) return true;
                return target[prefix.size()] == '/' || target[prefix.size()] == '\\';
            }

            std::vector<StringView> components() const{
                std::vector<StringView> result;
                size_t pos = 0;
                while ((pos = getSelf().str().find('/', pos)) != String::npos) {
                    result.push_back(getSelf().str().substr(pos + 1));
                    pos++;
                }
                if (!getSelf().str().empty())
                    result.push_back(getSelf().str());
                return result;
            }

            void setComponents(const std::vector<StringView>& components){
                getSelf().str().clear();
                for (const auto& component : components) {
                    getSelf().str() += component;
                    if (!components.empty() && component != components.back())
                        getSelf().str() += '/';
                }
            }

            PathBase relativeTo(const PathBase& other) const{
                const auto& full = this->components(); 
                const auto& base = other.components();

                if (base.size() > full.size())
                    throw std::runtime_error("Base path longer than target path");

                for (size_t i = 0; i < base.size(); ++i) {
                    if (full[i] != base[i])
                        throw std::runtime_error("Base path is not a prefix of target path");
                }

                PathBase result;
                result.setComponents({full.begin() + static_cast<ssize_t>(base.size()), full.end()});
                return result;
            }

            decltype(auto) str() { return getSelf().str(); }
            decltype(auto) str() const { return getSelf().str(); }

            PathBase operator/(const PathBase& other){
                std::string lhs = getSelf().str();
                std::string rhs = other.getSelf().str();

                if (!lhs.empty() && lhs.back() == '/') lhs.pop_back();
                if (!rhs.empty() && rhs.front() == '/') rhs.erase(0, 1);

                return Derived(lhs + '/' + rhs);
            }

        protected:
            Derived& getSelf(){
                return static_cast<Derived&>(*this);
            }

            const Derived& getSelf() const{
                return static_cast<const Derived&>(*this);
            }
    };

    class PathView : public PathBase<PathView>{
        public:
            template<typename T>
            PathView(const PathBase<T>& path) : view{path.str()}{}
            PathView(const StringView& path) : view{path}{}
            
            StringView view;

        public:
            StringView str() const {return view;}
            StringView str() {return view;}
    };

    class Path : public PathBase<Path>{
        public:
            template<typename T>
            Path(const PathBase<T>& path) : storage{path.str()}{}
            Path(const String& path) : storage{path}{}

            String storage;

        public:
            const String& str() const {return storage;}
            String& str() {return storage;}
    };
}