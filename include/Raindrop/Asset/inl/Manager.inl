#pragma once
#include "../Manager.hpp"

namespace Raindrop::Core::Asset{
    inline Manager::Manager() : _impl{}{}
    inline Manager::Manager(std::shared_ptr<Internal::Asset::Manager> impl) : _impl{impl}{}

    inline const bool Manager::isValid() const noexcept{
        return _impl != nullptr;
    }

    inline void Manager::insertFactory(const std::type_info& info, std::shared_ptr<Factory>&& factory){
        assertGet().insertFactory(info, std::move(factory));
    }

    template<typename Asset, typename T, typename... Args>
    inline std::shared_ptr<T> Manager::emplaceFactory(Args&&... args){
        return assertGet().emplaceFactory<Asset, T, Args...>(std::forward<Args>(args)...);
    }

    inline std::shared_ptr<Factory> Manager::getFactory(const std::type_info& typeInfo){
        return assertGet().getFactory(typeInfo);
    }

    inline std::shared_ptr<Factory> Manager::getFactory(const std::string& name){
        return assertGet().getFactory(name);
    }

    template<typename T>
    inline std::shared_ptr<T> Manager::getFactory(){
        return assertGet().getFactory<T>();
    }

    inline std::shared_ptr<Asset> Manager::loadOrGet(const std::type_info& typeInfo, const std::filesystem::path& path){
        return assertGet().loadOrGet(typeInfo, path);
    }

    inline std::shared_ptr<Asset> Manager::loadOrGet(const std::string& factoryName, const std::filesystem::path& path){
        return assertGet().loadOrGet(factoryName, path);
    }

    template<typename T>
    inline std::shared_ptr<T> Manager::loadOrGet(const std::filesystem::path& path){
        return assertGet().loadOrGet<T>(path);
    }

    inline Internal::Asset::Manager* Manager::get(){
        return _impl.get();
    }

    inline Internal::Asset::Manager& Manager::assertGet(){
        Internal::Asset::Manager* ptr = get();
        assert(ptr != nullptr);
        return *ptr;
    }
}