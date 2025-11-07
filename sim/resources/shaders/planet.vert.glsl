#version 450
#pragma shader_stage(vertex)

layout(push_constant, std430) uniform pc{
	mat4 transform;
	mat4 viewTransform;
};

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_UV;

layout (location = 0) out vec3 frag_color;

void main(){

	frag_color = vec3(1.0);
	gl_Position = viewTransform * transform * vec4(vert_pos, 1.0);
}