#include <Raindrop/Raindrop.hpp>
#include <Window/WindowModule.hpp>

class Test : public Raindrop::IModule{
    public:
        Test(){

        }

        virtual ~Test() override{

        }

        virtual Raindrop::Result initialize(Raindrop::InitHelper& init) override{

            return Raindrop::Result::Success();
        }

        void run(){
            spdlog::info("initialized ! ");

        }
};

int main(){
    Raindrop::Engine engine;

    auto& modules = engine.getModuleManager();
    modules.loadModules(engine.executableDirectory() / "modules/");

    modules.registerModule<Test>(
        "Test",
        Raindrop::Version(1, 0, 0),
        Raindrop::DependencyList{
            Raindrop::Dependency("Window", Raindrop::VersionConstraint::Parse("*"))
        },
        true
    );

    engine.initialize();

    if (auto module = modules.getModuleAs<Test>("Test"); module != nullptr){
        module->run();
    }
}