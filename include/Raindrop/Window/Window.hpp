#pragma once

#include "Raindrop/Window/Context.hpp"
#include "Raindrop/Window/Property.hpp"
#include "../Engine.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

namespace Raindrop::Window{
	class Window{
		public:
			Window(Engine& engine);
			~Window();

			glm::ivec2 getSize() const;
			glm::ivec2 getPosition() const;

			const char* getTitle() const;

			bool isFullscreen() const;
			bool isMinimized() const;
			bool isMaximized() const;

			bool resized() const;
			bool moved() const;
			
			void setSize(glm::ivec2 size);
			void setPosition(glm::ivec2 position);

			void* getNativeHandle() const;

			template<typename T, typename... Args>
			inline std::shared_ptr<T> addProperty(Args&&... args);

			template<typename T>
			inline std::shared_ptr<T> addProperty();

			template<typename T>
			inline std::shared_ptr<T> getProperty();

			template<typename T>
			inline const std::shared_ptr<T> getProperty() const;

			template<typename T>
			inline bool hasProperty() const noexcept;

			template<typename T>
			inline void removeProperty() noexcept;

		protected:
			Engine& _engine;
			void* _handle;
			std::unordered_map<std::size_t, std::weak_ptr<Property>> _properties;

	};
}

#include "inl/Window.inl"