#version 450
#pragma shader_stage(vertex)

struct Vertex{
	vec2 position;
	vec3 color;
};

Vertex vertices[] = {
	{{0, -0.5}, {1.0, 0.0, 0.0}},
	{{0.5, 0.5}, {0.0, 1.0, 0.0}},
	{{-0.5, 0.5}, {0.0, 0.0, 1.0}}
};

layout (location = 0) out vec3 frag_color;

void main(){
	const Vertex vertex = vertices[gl_VertexIndex];

	frag_color = vertex.color;
	gl_Position = vec4(vertex.position, 0.0, 1.0);
}