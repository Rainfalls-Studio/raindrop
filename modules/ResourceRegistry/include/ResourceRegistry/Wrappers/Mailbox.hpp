#pragma once

#include <vector>
#include <mutex>
#include <utility>

namespace ResourceRegistry{

    template<typename Cmd>
    class Mailbox{
        public:
            using ValueType = Cmd;

            void submit(Cmd cmd){
                std::scoped_lock lock(_mtx);
                _queue.push_back(std::move(cmd));
            }
            
            std::vector<Cmd> drain() {
                std::vector<Cmd> out;
                {
                    std::scoped_lock lock(_mtx);
                    out.swap(_queue);
                }
                return out;
            }

            void drain_into(std::vector<Cmd>& out) {
                out.clear();
                {
                    std::scoped_lock lock(_mtx);
                    out.swap(_queue);
                }
            }

            size_t size() const {
                std::scoped_lock lock(_mtx);
                return _queue.size();
            }

        private:
            mutable std::mutex _mtx;
            std::vector<Cmd> _queue;

    };
}