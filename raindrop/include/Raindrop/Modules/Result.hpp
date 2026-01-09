#pragma once

#include <string>

namespace Raindrop{
    class Result{
        public:
            enum class Level{

                // Engine wide error. A fatal error that will cause the engine to stop.
                FATAL,

                // Module failure. The module will be marked as failed and it's dependents will be stoped
                ERROR,

                // Successfull initialization. 
                SUCCESS
            };

            Result(Level level, std::string message = "") : _level(level), _message(message) {}

            static inline Result Fatal(const std::string& message = ""){
                return Result(Level::FATAL, message);
            }

            static inline Result Error(const std::string& message = ""){
                return Result(Level::ERROR, message);
            }

            static inline Result Success(const std::string& message = ""){
                return Result(Level::SUCCESS, message);
            }
            
            inline Level level() const noexcept {return _level;}
            inline const std::string& message() const noexcept {return _message;}

        private:
            Level _level;
            std::string _message;
    };
}