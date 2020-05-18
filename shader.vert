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
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec3 fragEye;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragPos;
layout(location = 4) out vec3 fragCamPos;

void main() {
  mat3 normalMatrix = transpose(inverse(mat3(ubo.model)));
  fragNormal = normalize(normalMatrix * inNormal);
  fragPos = vec3(ubo.model * vec4(inPosition, 1.0));
  fragEye = vec3(-fragPos);
  fragTexCoord = inTexCoord;
  fragCamPos = ubo.camPos;
  gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
}