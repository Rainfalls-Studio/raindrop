#include "Raindrop/Core/Tasks/TaskHandle.hpp"

namespace Raindrop::Tasks{
    TaskHandle::TaskHandle(std::shared_ptr<TaskDef> d) : def(std::move(d)) {}

    TaskHandle TaskHandle::then(const TaskHandle& next) {
        def->chained.push_back(next.def);
        return next;
    }
}