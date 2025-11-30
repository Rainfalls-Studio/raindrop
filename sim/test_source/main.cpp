#include <Raindrop/Raindrop.hpp>
#include <dlfcn.h>

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

        virtual Raindrop::Modules::Name name() const noexcept override{
            return "Test";
        }
};

int main(){
    Raindrop::Engine engine;

    spdlog::info("{}", engine.executableDirectory().string());

    // engine.loadModules("modules/")
    auto& modules = engine.getModuleManager();
    modules.loadModules(engine.executableDirectory() / "modules/");

    // void* handle = dlopen("/home/alexismagnier/Dev/C++/raindrop/build/dev/modules/Test/clean/bin/linux/Test.so", RTLD_NOW);
    // if (!handle){
        // spdlog::error("{}", dlerror());
    // }

    // modules.registerModule<Test>();
}