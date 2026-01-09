#include "Raindrop/Modules/Errors.hpp"
#include <system_error>

namespace Raindrop{
    class SystemErrorCategory : public std::error_category{
        public:
            const char* name() const noexcept override{
                return "System";
            }

            std::string message(int ev) const override{
                switch (static_cast<ErrorCodes>(ev)){
                    case ErrorCodes::CYCLIC: return "Cyclic dependency";
                    case ErrorCodes::MISSING_DEPENDENCY: return "Missing dependency";
                    case ErrorCodes::NOT_REGISTRED: return "Not registered";
                    case ErrorCodes::TYPE_MISSMATCH: return "Type missmatch";
                    default: return "Unknown system error";
                }
            }
    };

    const std::error_category& system_error_category(){
        static SystemErrorCategory category;
        return category;
    }
}