#pragma once

#include "Raindrop/Core/Internal/Asset/Manager.hpp"
#include "Factory.hpp"
#include "Asset.hpp"

namespace Raindrop::Core::Asset{

    class Manager{
        public:
            inline Manager();
            inline Manager(std::shared_ptr<Internal::Asset::Manager> impl);
            ~Manager() = default;

            static Manager Create();

            inline const bool isValid() const noexcept;

            inline void insertFactory(const std::type_info& info, std::shared_ptr<Factory>&& factory);

            template<typename Asset, typename T, typename... Args>
            inline std::shared_ptr<T> emplaceFactory(Args&&... args);

			inline std::shared_ptr<Factory> getFactory(const std::type_info& typeInfo);

            inline std::shared_ptr<Factory> getFactory(const std::string& name);

            template<typename T>
            inline std::shared_ptr<T> getFactory();


            inline std::shared_ptr<Asset> loadOrGet(const std::type_info& typeInfo, const std::filesystem::path& path);

            inline std::shared_ptr<Asset> loadOrGet(const std::string& factoryName, const std::filesystem::path& path);

            template<typename T>
            inline std::shared_ptr<T> loadOrGet(const std::filesystem::path& path);

            inline Internal::Asset::Manager* get();

        private:
            std::shared_ptr<Internal::Asset::Manager> _impl;

            inline Internal::Asset::Manager& assertGet();
    };
}

#include "inl/Manager.inl"