#ifndef __VULKANMODULE_PLUGINS_DEBUG_RENDERER_COMPONENT_HPP__
#define __VULKANMODULE_PLUGINS_DEBUG_RENDERER_COMPONENT_HPP__

#include "include/core.hpp"
#include "DebugRenderer.hpp"

class DebugComponent{
	public:
		DebugComponent(DebugRenderer& renderer);
		~DebugComponent();

		void drawLine(const maths::vec3<float32>& start, const maths::vec3<float32>& end, const maths::vec4<float32>& color = maths::vec4<float32>(1.f));
		void drawSquare(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
		void drawTrigone(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
		void drawCircle(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
		void drawCube(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
		void drawSphere(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
		void drawPyramid(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);

	private:
		DebugRenderer& _renderer;
};

#endif