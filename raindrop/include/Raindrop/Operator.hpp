#pragma once

#include <cassert>
#include <functional>

namespace Raindrop{
    class CompareOperator{
        public:
            enum Op{
                LESS,
                LESS_OR_EQUAL,
                EQUAL,
                NOT_EQUAL,
                GREATER_OR_EQUAL,
                GREATER,
                TRUE,
                FALSE
            };

            CompareOperator(Op op) noexcept : _op{op}{}

            CompareOperator(const CompareOperator&) = default;
            CompareOperator(CompareOperator&&) = default;

            CompareOperator& operator=(const CompareOperator&) = default;
            CompareOperator& operator=(CompareOperator&&) = default;

            template<typename T>
            std::function<bool(const T&, const T&)> getComparator() const noexcept{
                switch (_op){
                    case LESS:              return std::less<T>();
                    case LESS_OR_EQUAL:     return std::less_equal<T>();
                    case EQUAL:             return std::equal_to<T>();
                    case GREATER_OR_EQUAL:  return std::greater_equal<T>();
                    case GREATER:           return std::greater<T>();
                    case NOT_EQUAL:         return std::not_equal_to<T>();
                    case TRUE:              return [](const T&, const T&) -> bool {return true;};
                    case FALSE:             return [](const T&, const T&) -> bool {return false;};
                }
                assert(false && "Unknown comparator op");
            }

            inline void setOperator(Op op) noexcept{
                _op = op;
            }

            inline Op getOperator() const noexcept{
                return _op;
            }

        private:
            Op _op;
    };
}