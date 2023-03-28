#include "DebugRenderer.hpp"
#include "Component.hpp"

DebugRenderer::DebugRenderer(gfx::Context& context, scene::Scene& scene) : _context{static_cast<Context&>(context)}, _scene{scene}{
	RAINDROP_profile_function();
	_scene.registerComponent<DebugComponent>();
}

DebugRenderer::~DebugRenderer(){
	RAINDROP_profile_function();

}

const char* DebugRenderer::getName() const{
	RAINDROP_profile_function();
	return "DEBUG_RENDERER";
}

version DebugRenderer::getVersion() const{
	RAINDROP_profile_function();
	
}

RAINDROP_MODULE gfx::Plugin* PLUGIN_DEBUG_RENDERER(Allocator &allocator, gfx::Context& context, scene::Scene& scene){
	return static_cast<gfx::Plugin*>(allocateNew<DebugRenderer>(allocator, context, scene));
}

void DebugRenderer::drawLine(const maths::vec3<float32>& start, const maths::vec3<float32>& end, const maths::vec4<float32>& color){
	RAINDROP_profile_function();

}

void DebugRenderer::drawSquare(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();

}

void DebugRenderer::drawTrigone(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();

}

void DebugRenderer::drawCircle(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();
}

void DebugRenderer::drawCube(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();

}

void DebugRenderer::drawSphere(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();

}

void DebugRenderer::drawPyramid(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();
}
