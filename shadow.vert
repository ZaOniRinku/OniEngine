#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10

layout(binding = 0) uniform UniformBufferObject {
  mat4 model;
  mat4 view;
  mat4 proj;
  vec3 camPos;
} ubo;

layout(location = 0) in vec3 inPosition;

void main() {
  mat4 lightSpace = ubo.proj * ubo.view;
  gl_Position = lightSpace * ubo.model * vec4(inPosition, 1.0);
}