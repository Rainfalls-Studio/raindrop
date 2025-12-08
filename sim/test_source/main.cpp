#include <Raindrop/Raindrop.hpp>

class Test : public Raindrop::Modules::IModule{
    public:
        Test(){

        }

        virtual ~Test() override{

        }

        virtual Raindrop::Modules::Result initialize(Raindrop::Modules::InitHelper& init [[__maybe_unused__]]) override{
            spdlog::info("initialized ! ");
            return Raindrop::Modules::Result::Success();
        }
};

int main(){
    Raindrop::Engine engine;

    spdlog::info("{}", engine.executableDirectory().string());

    auto& modules = engine.getModuleManager();
    modules.loadModules(engine.executableDirectory() / "modules/");

    // modules.registerModule<Test>();
}