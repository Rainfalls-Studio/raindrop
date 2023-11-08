#include <common.hpp>
#include <Core/Scene/Context.hpp>
#include <Raindrop/Core/Scene/ComponentManager.hpp>

static constexpr std::size_t componentCount = 1000;
using Component = int;

class CoreSceneComponentManagerFixture : public CoreSceneContextFixture{
	public:
		CoreSceneComponentManagerFixture() : 
			manager{context, sizeof(Component), typeid(Component).hash_code(), componentCount,
				[](void* arg){*reinterpret_cast<Component*>(arg) = 0;},
				[](void* arg){*reinterpret_cast<Component*>(arg) = 0;}}
		{
			static_assert(std::is_arithmetic<Component>::value, "Cannot test a non arithemic component here");
		}
		
		::Raindrop::Core::Scene::ComponentManager manager;
};

// CONSTRUCTOR
TEST_F(CoreSceneContextFixture, ComponentManager_Constructor_valid){
	EXPECT_NO_THROW(::Raindrop::Core::Scene::ComponentManager(context, 5, 5, 5, [](void*){return;}, [](void*){return;}));
}

TEST_F(CoreSceneContextFixture, ComponentManager_Constructor_invalidComponentSize){
	EXPECT_DEATH(::Raindrop::Core::Scene::ComponentManager(context, 0, 5, 5, [](void*){return;}, [](void*){return;}), "");
}

TEST_F(CoreSceneContextFixture, ComponentManager_Constructor_invalidComponentCount){
	EXPECT_DEATH(::Raindrop::Core::Scene::ComponentManager(context, 5, 5, 0, [](void*){return;}, [](void*){return;}), "");
}

// METHODS
TEST_F(CoreSceneComponentManagerFixture, valid_getFirstComponent){
	EXPECT_NE(manager.get(0), nullptr);
}

TEST_F(CoreSceneComponentManagerFixture, valid_getLastComponent){
	EXPECT_NE(manager.get(componentCount-1), nullptr);
}

TEST_F(CoreSceneComponentManagerFixture, valid_getMidComponent){
	EXPECT_NE(manager.get(componentCount/2), nullptr);
}

TEST_F(CoreSceneComponentManagerFixture, invalid_getComponent){
	EXPECT_THROW(manager.get(componentCount*2), std::out_of_range);
}

TEST_F(CoreSceneComponentManagerFixture, size){
	EXPECT_EQ(manager.size(), componentCount);
}

TEST_F(CoreSceneComponentManagerFixture, array){
	EXPECT_NE(manager.array(), nullptr);
}

TEST_F(CoreSceneComponentManagerFixture, array_write){
	static constexpr Component value = 1;

	*reinterpret_cast<Component*>(manager.get(0)) = value;
	EXPECT_EQ(reinterpret_cast<Component*>(manager.array())[0], value);
}

TEST_F(CoreSceneComponentManagerFixture, valid_entityRegistration){
	EXPECT_NO_THROW(manager.addEntity(::Raindrop::Core::Scene::EntityID(0)));
}

// HANDLE CREATION
class ComponentManagerHandleFixture : public CoreSceneComponentManagerFixture{
	public:
		::Raindrop::Core::Scene::ComponentHandleID handle;
};

TEST_F(ComponentManagerHandleFixture, valid_createComponent){
	EXPECT_NO_THROW(handle = manager.createComponent());
}

TEST_F(ComponentManagerHandleFixture, valid_destroyComponenent){
	EXPECT_NO_THROW(handle = manager.createComponent());
	EXPECT_NO_THROW(manager.destroyComponent(handle));
}

TEST_F(ComponentManagerHandleFixture, invalid_doubleDestroyComponent){
	EXPECT_NO_THROW(handle = manager.createComponent());
	EXPECT_NO_THROW(manager.destroyComponent(handle));
	EXPECT_ANY_THROW(manager.destroyComponent(handle));
}

// ENTITY REGISTRATION
class ComponentManagerEntityFixture : public CoreSceneComponentManagerFixture{
	public:
		ComponentManagerEntityFixture() : entity{0}{}
		::Raindrop::Core::Scene::EntityID entity;

		virtual void SetUp() override{
			CoreSceneComponentManagerFixture::SetUp();
			manager.addEntity(entity);
		}
};

TEST_F(ComponentManagerEntityFixture, entityPresence){
	auto& list = manager.entities();

	EXPECT_FALSE(list.empty());
	auto it = std::find(list.begin(), list.end(), entity);
	EXPECT_NE(it, list.end());
}

TEST_F(ComponentManagerEntityFixture, valid_entityUnregistration){
	EXPECT_NO_THROW(manager.removeEntity(entity));
}

TEST_F(ComponentManagerEntityFixture, invalid_doubleEntityUnregistrayion){
	EXPECT_NO_THROW(manager.removeEntity(entity));
	EXPECT_THROW(manager.removeEntity(entity), std::runtime_error);
}