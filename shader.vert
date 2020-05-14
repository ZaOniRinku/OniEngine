#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
  mat4 model;
  mat4 view;
  mat4 proj;
  float ambientLightValue;
  vec3 ambientLightColor;
  vec3 lightPosition;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec3 fragEye;
layout(location = 2) out vec3 fragLightDir;
layout(location = 3) out vec2 fragTexCoord;
layout(location = 4) out vec3 fragAmbient;
layout(location = 5) out float fragDistance;

void main() {
  mat3 normalMatrix = transpose(inverse(mat3(ubo.model)));
  fragNormal = normalize(normalMatrix * inNormal);
  vec4 pos = ubo.model * vec4(inPosition, 1.0);
  fragLightDir = ubo.lightPosition - vec3(pos);
  fragEye = vec3(-pos);
  fragTexCoord = inTexCoord;
  fragAmbient = ubo.ambientLightColor * ubo.ambientLightValue;
  gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
  fragDistance = length(vec3(pos) - ubo.lightPosition);
}
