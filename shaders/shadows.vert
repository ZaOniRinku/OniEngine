#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
} ubo;

layout(binding = 1) uniform ShadowBufferObject {
	mat4 lightSpace[MAX_DIR_LIGHTS];
} sbo;

layout(push_constant) uniform LightIndex {
	int lightIndex;
} li;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBitangent;

void main() {
	gl_Position = sbo.lightSpace[li.lightIndex] * ubo.model * vec4(inPosition, 1.0);
	gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}