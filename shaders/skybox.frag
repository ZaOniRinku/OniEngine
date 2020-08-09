#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragTexCoords;

layout(binding = 5) uniform sampler2D diffuseTexSampler;

layout(location = 0) out vec4 outColor;

void main() {
	vec4 diffuse = texture(diffuseTexSampler, fragTexCoords);
	
	outColor = vec4(vec3(diffuse), 1.0);
}