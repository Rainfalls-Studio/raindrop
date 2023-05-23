#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>
#include <Raindrop/Core/Asset/Asset.hpp>
#include <gtest/gtest.h>

using namespace Raindrop::Core::Asset;

class AssetManagerTest : public ::testing::Test{
	public:
		virtual ~AssetManagerTest() override = default;

		static AssetManager _assetManager;
};

AssetManager AssetManagerTest::_assetManager;

class AssetTest : public Asset{
	public:
		AssetTest() = default;
		virtual ~AssetTest() override = default;

		int get1() const{
			return 1;
		}
};

class AssetFactoryTest : public AssetFactory{
	public:
		virtual ~AssetFactoryTest() override = default;
		
		virtual std::shared_ptr<Asset> createAsset(const std::filesystem::path& path) override{
			return std::static_pointer_cast<Asset>(std::make_shared<AssetTest>());
		}

		virtual void destroyAsset(std::shared_ptr<Asset> asset) override{
			asset.reset();
		}
};

TEST_F(AssetManagerTest, valid_factory_linking){
	std::shared_ptr<AssetFactoryTest> factory = std::make_unique<AssetFactoryTest>();
	_assetManager.linkFactory(".txt", std::static_pointer_cast<AssetFactory>(factory));
}

TEST_F(AssetManagerTest, load_valid_asset){
	auto asset = _assetManager.loadOrGet<AssetTest>("asset.txt");
	EXPECT_EQ(asset->get1(), 1);
}