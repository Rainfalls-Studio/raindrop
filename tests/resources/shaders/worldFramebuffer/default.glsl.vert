#version 450

layout (location = 0) out vec2 frag_UV;

struct Vertex{
	vec2 pos;
	vec2 UV;
};

Vertex vertices[] = {
	Vertex(vec2(-1.0, -1.0), vec2(1, 1)),
	Vertex(vec2( 1.0, -1.0), vec2(0, 1)),
	Vertex(vec2( 1.0,  1.0), vec2(0, 0)),

	Vertex(vec2(-1.0, -1.0), vec2(1, 1)),
	Vertex(vec2(-1.0,  1.0), vec2(1, 0)),
	Vertex(vec2( 1.0,  1.0), vec2(0, 0)),
};

void main(){
	frag_UV = vertices[gl_VertexIndex].UV;
	gl_Position = vec4(vertices[gl_VertexIndex].pos, 0.0, 1.0);
}