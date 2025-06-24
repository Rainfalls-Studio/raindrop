#include "Raindrop/Event/Manager.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/spdlog.h>
#include <exception>

namespace Raindrop::Event{
	Manager::Manager(Engine& engine) noexcept : _engine{engine}{
		_createLogger();

		SPDLOG_LOGGER_INFO(_logger, "Manager successfully created !");
	}

	Manager::~Manager(){}
	
	void Manager::subscribe(const std::size_t& id, const CallbackInfo& info){
		_events[id].callbacks.push_back(info);
	}

	void Manager::trigger(const std::size_t& id, const Event& event){
		auto& callbacks = _events[id].callbacks;
		for (const auto& callback : callbacks){
			try{
				callback.callback(event);
			} catch (const std::exception& e){
				SPDLOG_LOGGER_WARN(_logger, "Callback for event \"{}\" throwed an exception : {}", id, e.what());
			}
		}
	}

    void Manager::_createLogger(){
        _logger = spdlog::get("Raindrop::Event");
        if (!_logger){
            _logger = spdlog::stdout_color_mt("Raindrop::Event");
        }
    }
}