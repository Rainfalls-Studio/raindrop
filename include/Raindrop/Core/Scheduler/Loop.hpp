#pragma once

#include "LoopData.hpp"

namespace Raindrop::Scheduler{
    class Loop{
        public:
            inline Loop(std::shared_ptr<LoopData> data = {}) : _data{data}{}

            template<typename Stage, typename... Args>
            inline Loop& addStage(Args&&... args) {
                _data->addStage<Stage>(std::forward<Args>(args)...);
                return *this;
            }

            inline Loop& setPeriod(const Time::Duration& period) {
                _data->setPeriod(period);
                return *this;
            }

            inline Loop& setExecutionPriority(int priority){
                _data->setExecutionPriority(priority);
                return *this;
            }
            
            inline bool valid() const noexcept{
                return _data != nullptr;
            }

            inline const std::string& name() const noexcept{
                return _data->name;
            }

            inline operator bool() const noexcept{
                return valid();
            }

            const std::shared_ptr<LoopData>& data() const noexcept{
                return _data;
            }

        private:
            std::shared_ptr<LoopData> _data;

            LoopData& getData(){
                if (!_data) throw std::runtime_error("Loop data not set");
                return *_data;
            }
    };
}