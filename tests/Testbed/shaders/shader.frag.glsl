#version 450
#pragma shader_stage(fragment)

layout (location = 0) in vec3 in_color;
layout (location = 1) in vec3 in_position;

layout (location = 0) out vec4 out_color;

void main(){
	out_color = vec4(1.0);
}