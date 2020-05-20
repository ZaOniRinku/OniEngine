#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10

layout(binding = 1) uniform Lights {
	int fragNumDirLights;
	vec3 fragDirLights[MAX_DIR_LIGHTS];
	vec3 fragDirLightsColor[MAX_DIR_LIGHTS];
	int fragNumPointLights;
	vec3 fragPointLights[MAX_POINT_LIGHTS];
	vec3 fragPointLightsColor[MAX_POINT_LIGHTS];
} lights;

layout(binding = 2) uniform sampler2D diffuseTexSampler;
layout(binding = 3) uniform sampler2D normalTexSampler;
layout(binding = 4) uniform sampler2D metallicTexSampler;
layout(binding = 5) uniform sampler2D roughnessTexSampler;
layout(binding = 6) uniform sampler2D AOTexSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 fragCamPos;
layout(location = 4) in mat3 fragTBN;

layout(location = 0) out vec4 outColor;

#define M_PI 3.1415926535897932384626433832795

float RDM_Distribution(float NdotH, float roughness) {
	float a = roughness * roughness;
	float asquare = a * a;
	float NdotHcarre = NdotH * NdotH;
	float denom = NdotHcarre * (asquare - 1.0) + 1.0;

	return asquare / (M_PI * denom * denom);
}

vec3 RDM_Fresnel(float costheta, vec3 f0) {
	return f0 + (1.0 - f0) * pow(1.0 - costheta, 5.0);
}

float RDM_G1(float NdotV, float roughness) {
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	float denom = NdotV * (1.0 - k) + k;

	return NdotV / denom;
}

float RDM_Smith(float LdotN, float VdotN, float roughness) {
	float g1l = RDM_G1(VdotN, roughness);
	float g1v = RDM_G1(LdotN, roughness);

	return g1l * g1v;
}

vec3 RDM_bsdf(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN, vec3 diffuse, float metallic, float roughness) {
	float d = RDM_Distribution(max(NdotH, 0.0), roughness);
	vec3 f = RDM_Fresnel(max(VdotH, 0.0), mix(vec3(0.04), diffuse, metallic));
	float g = RDM_Smith(max(LdotN, 0.0), max(VdotN, 0.0), roughness);

	vec3 specular = (d * f * g) / max(4.0 * max(LdotN, 0.0) * max(VdotN, 0.0), 0.001);
	vec3 kd = (vec3(1.0) - f) * (1.0 - metallic);
	return kd * diffuse / M_PI + specular;
}

vec3 shade(vec3 n, vec3 v, vec3 l, vec3 lc, vec3 diffuse, float metallic, float roughness) {
	vec3 h = normalize(v+l);
	float LdotH = dot(l,h);
	float NdotH = dot(n,h);
	float VdotH = dot(v,h);
	float LdotN = dot(l,n);
	float VdotN = dot(v,n);
	vec3 bsdf = RDM_bsdf(LdotH, NdotH, VdotH, LdotN, VdotN, diffuse, metallic, roughness);
	vec3 ret = lc*bsdf*LdotN;

	return ret;
}

void main() {
	vec4 diffuse = texture(diffuseTexSampler, fragTexCoord);
	vec3 normal = texture(normalTexSampler, fragTexCoord).xyz;
	float metallic = texture(metallicTexSampler, fragTexCoord).x;
	float roughness = texture(roughnessTexSampler, fragTexCoord).x;
	float ao = texture(AOTexSampler, fragTexCoord).x;

	vec3 d = vec3(diffuse);
	vec3 n = normal * 2.0 - 1.0;
	n = normalize(fragTBN * n);
	vec3 v = normalize(fragCamPos - fragPos);
	vec3 l;
	
	vec3 color = vec3(0.0);
	for (int i = 0; i < lights.fragNumDirLights; i++) {
		l = normalize(-lights.fragDirLights[i]);
		color += shade(n, v, l, lights.fragDirLightsColor[i], d, metallic, roughness);
	}
	for (int i = 0; i < lights.fragNumPointLights; i++) {
		l = normalize(lights.fragPointLights[i] - fragPos);
		float distance = length(lights.fragPointLights[i] - fragPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lights.fragPointLightsColor[i] * attenuation;
		color += shade(n, v, l, radiance, d, metallic, roughness);
	}

	vec3 ambient = vec3(0.03) * d * ao;
	color += ambient;

	// HDR
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));
	outColor = vec4(color, 1.0);
}