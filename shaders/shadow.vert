#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
  mat4 model;
} ubo;

layout(binding = 3) uniform ShadowBufferObject {
  mat4 view;
  mat4 proj;
  vec3 pos;
} sbo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBitangent;

void main() {
  gl_Position = sbo.proj * sbo.view * ubo.model * vec4(inPosition, 1.0);
}