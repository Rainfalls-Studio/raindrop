#include "Raindrop/Window/Manager.hpp"
#include "Raindrop/Window/Config.hpp"
#include <memory>

namespace Raindrop::Window{

    Manager::Manager(Engine& engine) : _engine{engine}{}

    std::shared_ptr<Window> Manager::createWindow(const Config& config){
        auto window = std::make_shared<Window>(config);

        _windows.push_back(window);

        return window;
    }

    void Manager::foreach(std::function<void(Window&)> callback){
        auto it = _windows.begin();

        while (it != _windows.end()){
            auto window = it->lock();

            if (!window){
                it = _windows.erase(it);
                continue;
            }

            callback(*window);
            it++;
        }
    }

    const std::list<std::weak_ptr<Window>>& Manager::getWindows(){
        return _windows;
    }

    std::size_t Manager::getSize() const{
        return _windows.size();
    }
}