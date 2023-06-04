#version 450

layout (location = 0) out vec3 fragColor;

layout (push_constant) uniform Push{
	mat4 viewTransform;
	mat4 localTransform;
} push;

struct Vertex{
	vec3 position;
	vec3 color;
};

Vertex vertices[6] = Vertex[](
	Vertex(vec3(-1, -1, 0.5), vec3(1.0, 1.0, 0.0)),
	Vertex(vec3(-1, 1, 0.5), vec3(1.0, 1.0, 0.0)),
	Vertex(vec3(1, 1, 0.5), vec3(0.0, 1.0, 1.0)),
	
	Vertex(vec3(-1, -1, 0.5), vec3(1.0, 1.0, 0.0)),
	Vertex(vec3(1, 1, 0.5), vec3(1.0, 1.0, 0.0)),
	Vertex(vec3(1, -1, 0.5), vec3(0.0, 1.0, 1.0))
);

void main(){
	fragColor = vertices[gl_VertexIndex].color;
	gl_Position = vec4(vertices[gl_VertexIndex].position, 1.0) * push.localTransform * push.viewTransform;
}