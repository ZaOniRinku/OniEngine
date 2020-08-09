#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

layout(binding = 0) uniform ObjectBufferObject {
	mat4 model;
} obo;

layout(binding = 1) uniform CameraBufferObject {
	mat4 view;
	mat4 proj;
	vec3 pos;
} cbo;

layout(binding = 3) uniform ShadowsBufferObject {
	vec3 numLights;
	mat4 dirLightsSpace[MAX_DIR_LIGHTS];
	mat4 spotLightsSpace[MAX_SPOT_LIGHTS];
} sbo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBitangent;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoords;
layout(location = 2) out vec3 fragPos;
layout(location = 3) out vec3 fragCamPos;
layout(location = 4) out vec4 fragDirLightsSpace[MAX_DIR_LIGHTS];
layout(location = MAX_DIR_LIGHTS + 4) out vec4 fragSpotLightsSpace[MAX_SPOT_LIGHTS];
layout(location = MAX_SPOT_LIGHTS + MAX_DIR_LIGHTS + 4) out mat3 fragTBN;

void main() {
	mat3 normalMatrix = transpose(inverse(mat3(obo.model)));
	fragNormal = normalize(normalMatrix * inNormal);
	fragPos = vec3(obo.model * vec4(inPosition, 1.0));
	fragTexCoords = inTexCoords;
	fragCamPos = cbo.pos;
	for (int i = 0; i < sbo.numLights.x; i++) {
		fragDirLightsSpace[i] = sbo.dirLightsSpace[i] * vec4(fragPos, 1.0);
	}
	for (int i = 0; i < sbo.numLights.z; i++) {
		fragSpotLightsSpace[i] = sbo.spotLightsSpace[i] * vec4(fragPos, 1.0);
	}
	vec3 T = normalize(vec3(obo.model * vec4(inTangent, 0.0)));
	vec3 B = normalize(vec3(obo.model * vec4(inBitangent, 0.0)));
	vec3 N = normalize(vec3(obo.model * vec4(inNormal, 0.0)));
	fragTBN = mat3(T, B, N);
	gl_Position = cbo.proj * cbo.view * vec4(fragPos, 1.0);
}