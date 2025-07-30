#pragma once

#include <system_error>
#include <format>

namespace Raindrop{
    template<typename... Args>
    struct MakeError;

    class Error{
        public:
            inline Error(
                std::error_code code,
                const std::string& reason = {}
            ) noexcept :
                _code{code},
                _reason{reason}
            {}


            template<typename... Args>
            inline Error(
                std::error_code code,
                std::string_view fmt,
                Args&&... args
            ) noexcept : 
                Error(
                    code,
                    std::vformat(fmt, std::make_format_args(args...))
                )
            {}

            inline std::error_code code() const noexcept{
                return _code;
            }

            inline std::string message() const noexcept{
                return _code.message();
            }
            
            inline const std::string& reason() const noexcept{
                return _reason;
            }

            template<typename... Args>
            using Make = MakeError<Args...>;

        private:
            std::error_code _code;
            std::string _reason;
    };
}