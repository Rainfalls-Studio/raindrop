#include <Raindrop/Raindrop.hpp>
#include <gtest/gtest.h>

TEST(sceneWrapper, valid_entity_creation){
	Raindrop::Core::Scene::Scene scene_ptr(RAINDROP_default_allocator, 1000);
	Raindrop::Scene scene(&scene_ptr);

	Raindrop::Entity entity = scene.createEntity();
	EXPECT_NE(entity, Raindrop::INVALID_ENTITY);
	EXPECT_TRUE(entity.exists());

	EXPECT_NO_THROW(scene.destroyEntity(entity));
}

struct TestComponent{
	Raindrop::Core::Maths::vec2<float> position;
	Raindrop::Core::Maths::vec2<float> scale;
	const char name[255];
};

TEST(SceneWrapper, valid_component_register){
	Raindrop::Core::Scene::Scene scene_ptr(RAINDROP_default_allocator, 1000);
	Raindrop::Scene scene(&scene_ptr);

	EXPECT_NO_THROW(scene.registerComponent<TestComponent>());
	EXPECT_NE(scene.getComponentSignature<TestComponent>(), Raindrop::INVALID_COMPONENT);

	Raindrop::Entity entity = scene.createEntity();
	TestComponent& component = entity.addComponent<TestComponent>();
	EXPECT_TRUE(entity.hasComponent<TestComponent>());

	component.position.x = 50.f;
	component.position.y = 45.f;

	TestComponent& component2 = entity.getComponent<TestComponent>();
	EXPECT_EQ(component.position, component2.position);
}

class TestSystem : public Raindrop::SystemBase{
	public:
		TestSystem() :  SystemBase(){
			constructor = true;
		}

		~TestSystem(){}

		virtual void OnEntityCreate(Raindrop::Entity entity) override{
			entityCreate = true;
		}

		virtual void OnEntityDestroy(Raindrop::Entity entity) override{
			entityDestroy = true;
		}

		bool constructor = false;
		bool entityCreate = false;
		bool entityDestroy = false;
};

TEST(sceneWrapper, valid_system_creation){
	Raindrop::Core::Scene::Scene scene_ptr(RAINDROP_default_allocator, 1000);
	Raindrop::Scene scene(&scene_ptr);

	EXPECT_NO_THROW(scene.registerComponent<TestComponent>());
	EXPECT_NE(scene.getComponentSignature<TestComponent>(), Raindrop::INVALID_COMPONENT);

	Raindrop::Signature signature = scene.getComponentsSignature<TestComponent>();
	TestSystem& system = scene.createSystem<TestSystem>(signature);
	EXPECT_TRUE(system.constructor);

	Raindrop::Entity entity = scene.createEntity();
	TestComponent& component = entity.addComponent<TestComponent>();
	EXPECT_TRUE(system.entityCreate);

	scene.destroyEntity(entity);
	EXPECT_TRUE(system.entityDestroy);

	scene.destoySystem(system);
}