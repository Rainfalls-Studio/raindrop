#ifndef __VULKANMODULE_DEBUG_RENDERER_HPP__
#define __VULKANMODULE_DEBUG_RENDERER_HPP__

#include "include/core.hpp"
#include "include/Context.hpp"

class DebugRenderer : public gfx::Plugin{
	public:
		DebugRenderer(gfx::Context& context, scene::Scene& scene);
		~DebugRenderer();

		virtual const char* getName() const override;
		virtual version getVersion() const override;

		void drawLine(const maths::vec3<float32>& start, const maths::vec3<float32>& end, const maths::vec4<float32>& color = maths::vec4<float32>(1.f));
		void drawSquare(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
		void drawTrigone(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
		void drawCircle(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
		void drawCube(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
		void drawSphere(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
		void drawPyramid(const scene::Components::Transform& transform, const maths::vec4<float32>& color = maths::vec4<float32>(1.f), bool filled = false);
	
	private:
		scene::Scene& _scene;
		Context& _context;
};

#endif