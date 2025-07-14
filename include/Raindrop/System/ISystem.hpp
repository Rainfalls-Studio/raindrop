#pragma once

#include <typeindex>
#include <vector>

namespace Raindrop{
    class Engine;
}

namespace Raindrop::System{
    class ISystem{
        public:
            struct Dependency{
                std::type_index type;

                // If marked optional. If the dependency is registred, it will initialize after it. If not, it will just skip it.
                bool optional = false;
            };

            virtual ~ISystem() = default;

            /**
             * @brief Called on system initialization. 
             * If dependencies are set, all required dependencies will be initialized before current system
             * If dependencies are marked optional, check availability before usage
             * 
             * @param engine the engine instance
             */
            virtual void initialize(Engine& engine);

            /**
             * @brief Called once all systems have been initialized
             * 
             */
            virtual void postInitialize();
            
            /**
             * @brief Called right before systems shutdown
             * 
             */
            virtual void preShutdown();

            /**
             * @brief Called on system shutdown.
             * If dependencieq are set, all required dependecies will be shutdown after current system and can still be accessed
             * 
             */
            virtual void shutdown();
            
            /**
             * @brief Called before any other step during the game loop
             * 
             */
            virtual void top();

            /**
             * @brief Called after every other steps during the game loop
             * 
             */
            virtual void bottom();

            /**
             * @brief Called before the event phase
             * 
             */
            virtual void preEvent();

            /**
             * @brief The event phase
             * 
             */
            virtual void event();

            /**
             * @brief Call right after the event phase
             * 
             */
            virtual void postEvent();
            
            /**
             * @brief Called before the update phase
             * 
             */
            virtual void preUpdate();

            /**
             * @brief The update phase
             * 
             */
            virtual void update();

            /**
             * @brief Called right after the update phase
             * 
             */
            virtual void postUpdate();
            
            /**
             * @brief Called before the render phase
             * 
             */
            virtual void preRender();

            /**
             * @brief The render phase
             * 
             */
            virtual void render();

            /**
             * @brief Called right after the render phase
             * 
             */
            virtual void postRender();

            /**
             * @brief The name of the system. Used for debug and info puroses
             * 
             * @return const char* 
             */
            virtual const char* name() const = 0;

            /**
             * @brief The list of system dependencies
             * 
             * @return std::vector<Dependency> 
             */
            virtual std::vector<Dependency> dependencies() const;
            
            /**
             * @brief Called right before any dependency system have been shutdown.
             * 
             * @param dependency The type index of the dependencies
             * @param system the dependency system instance
             */
            virtual void dependencyShutdown(const std::type_index& dependency, const ISystem& system);
    };
}