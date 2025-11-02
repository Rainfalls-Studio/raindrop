#version 450
#pragma shader_stage(fragment)

layout (location = 0) in vec3 vert_colors;

layout (location = 0) out vec4 out_color;

void main(){
	out_color = vec4(vert_colors, 1.0);
}