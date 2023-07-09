#version 450

layout (location = 0) in vec2 in_UV;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform sampler2D depth_tex;
layout (set = 0, binding = 1) uniform sampler2D albedo_tex;
layout (set = 0, binding = 2) uniform sampler2D bloom_tex;
layout (set = 0, binding = 3) uniform sampler2D normal_tex;
layout (set = 0, binding = 4) uniform sampler2D position_tex;

void main(){
	float depth = texture(depth_tex, in_UV).r;
	vec3 albedo = texture(albedo_tex, in_UV).rgb;
	float specular = texture(albedo_tex, in_UV).a;
	vec3 bloom = texture(bloom_tex, in_UV).rgb;
	vec3 normal = texture(normal_tex, in_UV).rgb;
	vec3 position = texture(position_tex, in_UV).rgb;

	outColor = vec4(albedo, 1.0);
}