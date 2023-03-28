#include "Component.hpp"

DebugComponent::DebugComponent(DebugRenderer& renderer) : _renderer{renderer}{
	RAINDROP_profile_function();

}

DebugComponent::~DebugComponent(){
	RAINDROP_profile_function();

}

void DebugComponent::drawLine(const maths::vec3<float32>& start, const maths::vec3<float32>& end, const maths::vec4<float32>& color){
	RAINDROP_profile_function();
	_renderer.drawLine(start, end, color);
}

void DebugComponent::drawSquare(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();
	_renderer.drawSquare(transform, color, filled);
}

void DebugComponent::drawTrigone(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();
	_renderer.drawPyramid(transform, color, filled);
}

void DebugComponent::drawCircle(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();
	_renderer.drawCircle(transform, color, filled);
}

void DebugComponent::drawCube(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();
	_renderer.drawCube(transform, color, filled);
}

void DebugComponent::drawSphere(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();
	_renderer.drawSphere(transform, color, filled);
}

void DebugComponent::drawPyramid(const scene::Components::Transform& transform, const maths::vec4<float32>& color, bool filled){
	RAINDROP_profile_function();
	_renderer.drawPyramid(transform, color, filled);
}
