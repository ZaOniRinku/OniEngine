#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragTexCoords;

layout(binding = 1) uniform samplerCube skyboxTexSampler;

layout(location = 0) out vec4 outColor;

void main() {
	outColor = texture(skyboxTexSampler, fragTexCoords);
}