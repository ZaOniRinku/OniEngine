#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10

layout(binding = 0) uniform ObjectBufferObject {
	mat4 model;
} obo;

layout(binding = 1) uniform CameraBufferObject {
	mat4 view;
	mat4 proj;
	vec3 pos;
} cbo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBitangent;

layout(location = 0) out vec2 fragTexCoord;

void main() {
	vec3 pos = vec3(obo.model * vec4(inPosition, 1.0));
	fragTexCoord = inTexCoord;
	gl_Position = cbo.proj * cbo.view * vec4(pos, 1.0);
}