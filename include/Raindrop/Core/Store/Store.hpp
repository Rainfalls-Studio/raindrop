#pragma once

#include <typeindex>
#include <spdlog/spdlog.h>
#include "Resource.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Store{
    struct IResourceHolder {
        virtual ~IResourceHolder() = default;
        virtual std::type_index type() const noexcept = 0;
    };

    template<typename T>
    struct ResourceHolder final : IResourceHolder {
        std::shared_ptr<Resource<T>> resource;
        explicit ResourceHolder(std::shared_ptr<Resource<T>> r) : resource(std::move(r)) {}
        std::type_index type() const noexcept override { return typeid(T); }
    };

    class Store {
    public:
        Store(Engine& engine);

        void shutdown(){
            spdlog::info("Shuting down store...");
            resources.clear();
        }

        template<typename T>
        ResourceReader<T> read(const std::string& name) {
            auto it = resources.find(name);
            if (it == resources.end()) throw std::runtime_error("resource not found: " + name);
            IResourceHolder* base = it->second.get();
            if (base->type() != typeid(T)) throw std::runtime_error("type mismatch for resource: " + name);
            auto* holder = static_cast<ResourceHolder<T>*>(base);
            return ResourceReader<T>(holder->resource);
        }

        template<typename T>
        ResourceWriter<T> emplace(const std::string& name,
                                const ResourceBuilder<T>& builder,
                                size_t bufferCount = 2)
        {
            // If already exists, throw
            if (resources.count(name)) throw std::runtime_error("resource already exists: " + name);
            auto resource = Resource<T>::create(builder, bufferCount);
            resources.emplace(name, std::make_unique<ResourceHolder<T>>(resource));

            auto maybeWriter = ResourceWriter<T>::make_writer(resource);
            if (!maybeWriter) throw std::runtime_error("failed to create writer for resource: " + name);
            return std::move(*maybeWriter);
        }

    private:
        Engine& _engine;
        std::unordered_map<std::string, std::unique_ptr<IResourceHolder>> resources;
    };
}