#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec2 in_UV;

layout (location = 0) out vec4 outColor;

void main(){
	outColor = vec4(in_color, 1.0);
}