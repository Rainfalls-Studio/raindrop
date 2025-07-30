#pragma once

#include <system_error>

namespace Raindrop::Systems{
    enum class ErrorCodes{

        // When a cyclic dependency has been found
        CYCLIC,

        // When a hard requirement is not met
        MISSING_DEPENDENCY,

        // When a required node has not been found
        NOT_REGISTRED,

        TYPE_MISSMATCH,

        // When 
        ALREADY_INITIALIZED
    };

    const std::error_category& system_error_category();

    inline std::error_code make_error_code(ErrorCodes e){
        return {static_cast<int>(e), system_error_category()};
    }
}

namespace std{
    template <>
    struct is_error_code_enum<Raindrop::Systems::ErrorCodes> : true_type{};
}