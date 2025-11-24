#version 450
#pragma shader_stage(vertex)

layout(push_constant, std430) uniform pc{
	mat4 transform;
	mat4 viewTransform;
};

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_UV;

layout (location = 2) in uint instance_face;
layout (location = 3) in vec2 instance_UV_offset;
layout (location = 4) in float instance_UV_scale;
layout (location = 5) in uint instance_packed_neighbors_lods;

layout (location = 0) out vec3 frag_color;

mat4 faceRotations[6] = mat4[](
    // -X
    mat4( 0, 0,-1, 0,
          0, 1, 0, 0,
          1, 0, 0, 0,
		 -0.5, 0, 0, 1
	),

    // +X
    mat4( 0, 0, 1, 0,
          0, 1, 0, 0,
         -1, 0, 0, 0,
		  0.5, 0, 0, 1
	),

    // -Y
    mat4( 1, 0, 0, 0,
          0, 0,-1, 0,
          0, 1, 0, 0,
		  0,-0.5, 0, 1
	),

    // +Y
    mat4( 1, 0, 0, 0,
          0, 0, 1, 0,
          0,-1, 0, 0,
		  0, 0.5, 0, 1
	),

    // -Z
    mat4( 1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0,-1, 0,
		  0, 0,-0.5, 1
	),

    // +Z (identity)
    mat4( 1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
		  0, 0, 0.5, 1
	)
);

void main(){
	vec4 localPos = faceRotations[instance_face] * vec4(vert_pos, 1.f);
	localPos.xyz /= length(localPos.xyz);

	vec3 normal = localPos.xyz;
	vec4 worldPos = transform * localPos;
	vec4 viewPos = viewTransform * worldPos;

	vec2 UV = vert_UV * instance_UV_scale + instance_UV_offset;

	frag_color = vec3(UV, 0.f);

	gl_Position = viewPos;
}