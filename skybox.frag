#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10

layout(binding = 1) uniform Lights {
	int fragNumDirLights;
	vec3 fragDirLights[MAX_DIR_LIGHTS];
	vec3 fragDirLightsColor[MAX_DIR_LIGHTS];
	int fragNumPointLights;
	vec3 fragPointLights[MAX_POINT_LIGHTS];
	vec3 fragPointLightsColor[MAX_POINT_LIGHTS];
} lights;

layout(binding = 2) uniform sampler2D diffuseTexSampler;
layout(binding = 3) uniform sampler2D normalTexSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragEye;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec4 fragPos;
layout(location = 4) in vec3 fragCamPos;

layout(location = 0) out vec4 outColor;

void main() {
	vec4 diffuse = texture(diffuseTexSampler, fragTexCoord);
	vec3 color = vec3(diffuse);
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));
	outColor = vec4(color, 1.0);
}