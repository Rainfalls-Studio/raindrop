#include <Raindrop/Raindrop.hpp>
#include <Window/WindowModule.hpp>

class Test : public Raindrop::Modules::IModule{
    public:
        Test(){

        }

        virtual ~Test() override{

        }

        virtual Raindrop::Modules::Result initialize(Raindrop::Modules::InitHelper& init) override{
            spdlog::info("initialized ! ");

            return Raindrop::Modules::Result::Success();
        }
};

int main(){
    Raindrop::Engine engine;

    auto& modules = engine.getModuleManager();
    modules.loadModules(engine.executableDirectory() / "modules/");

    modules.registerModule<Test>(
        "Test",
        Raindrop::Version(1, 0, 0),
        Raindrop::Modules::DependencyList{
            Raindrop::Modules::Dependency("Window")
        },
        true
    );

    // modules.getModuleAs<Test>("Test")
}