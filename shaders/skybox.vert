#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10

layout(binding = 0) uniform CameraBufferObject {
	mat4 view;
	mat4 proj;
	vec3 pos;
} cbo;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 fragTexCoords;

void main() {
	fragTexCoords = inPosition;
	fragTexCoords.x *= -1;
	vec4 pos = cbo.proj * vec4(mat3(cbo.view) * inPosition, 1.0);
	gl_Position = pos.xyww;
}