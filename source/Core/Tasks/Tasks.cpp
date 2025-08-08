#include "Raindrop/Core/Tasks/Task.hpp"

namespace Raindrop::Tasks{
    Task::Task(uint64_t id_, int prio, Func f, std::vector<uint64_t> d, std::string n)
            : id(id_), priority(prio), fn(std::move(f)), deps(std::move(d)), name(std::move(n)) {}
}