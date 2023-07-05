#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec2 in_UV;

layout (location = 0) out vec3 frag_position;
layout (location = 1) out vec3 frag_color;
layout (location = 2) out vec3 frag_normal;
layout (location = 3) out vec2 frag_UV;

layout (push_constant) uniform Push{
	mat4 projectionViewMatrix;
	mat4 modelMatrix;
} push;

void main(){
	frag_color = in_color;
	vec4 positionWorld = push.modelMatrix * vec4(in_position, 1.0);
	gl_Position = push.projectionViewMatrix * positionWorld;
}