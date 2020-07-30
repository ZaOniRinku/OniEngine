#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10

layout(binding = 2) uniform Lights {
	int fragNumDirLights;
	vec3 fragDirLights[MAX_DIR_LIGHTS];
	vec3 fragDirLightsColor[MAX_DIR_LIGHTS];
	int fragNumPointLights;
	vec3 fragPointLights[MAX_POINT_LIGHTS];
	vec3 fragPointLightsColor[MAX_POINT_LIGHTS];
} lights;

layout(binding = 4) uniform sampler2D shadowsTexSampler;
layout(binding = 5) uniform sampler2D diffuseTexSampler;
layout(binding = 6) uniform sampler2D normalTexSampler;
layout(binding = 7) uniform sampler2D metallicTexSampler;
layout(binding = 8) uniform sampler2D roughnessTexSampler;
layout(binding = 9) uniform sampler2D AOTexSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 fragCamPos;
layout(location = 4) in vec4 fragLightSpace;
layout(location = 5) in mat3 fragTBN;

layout(location = 0) out vec4 outColor;

#define M_PI 3.1415926535897932384626433832795

float distribution(float NdotH, float roughness) {
	float a = roughness * roughness;
	float asquare = a * a;
	float NdotHsquare = NdotH * NdotH;
	float denom = NdotHsquare * (asquare - 1.0) + 1.0;

	return asquare / (M_PI * denom * denom);
}

vec3 fresnel(float costheta, vec3 f0) {
	return f0 + (1.0 - f0) * pow(1.0 - costheta, 5.0);
}

float g1(float NdotV, float roughness) {
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	float denom = NdotV * (1.0 - k) + k;

	return NdotV / denom;
}

float smith(float LdotN, float VdotN, float roughness) {
	float g1l = g1(VdotN, roughness);
	float g1v = g1(LdotN, roughness);

	return g1l * g1v;
}

vec3 diffuseFresnelCorrection(vec3 n) {
	vec3 nsquare = n * n;
	bvec3 TIR = lessThan(n, vec3(1.0));
    vec3 invdenum = mix(vec3(1.0), vec3(1.0) / (nsquare * nsquare * (vec3(554.33) - 380.7 * n)), TIR);
    vec3 num = n * mix(vec3(0.1921156102251088), n * 298.25 - 261.38 * nsquare + 138.43, TIR);
    num += mix(vec3(0.8078843897748912), vec3(-1.67), TIR);
    return num * invdenum;
}

vec3 brdf(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN, vec3 diffuse, float metallic, float roughness) {
	float d = distribution(NdotH, roughness);
	vec3 f = fresnel(LdotH, mix(vec3(0.04), diffuse, metallic));
	vec3 fT = fresnel(LdotN, mix(vec3(0.04), diffuse, metallic));
	vec3 fTIR = fresnel(VdotN, mix(vec3(0.04), diffuse, metallic));
	float g = smith(LdotN, VdotN, roughness);
	vec3 dfc = diffuseFresnelCorrection(vec3(1.05));

	vec3 lambertian = diffuse / M_PI;
	return (d * f * g) / max(4.0 * LdotN * VdotN, 0.001) + ((vec3(1.0f) - fT) * (vec3(1.0f) - fTIR) * lambertian) * dfc;
}

vec3 shade(vec3 n, vec3 v, vec3 l, vec3 lc, vec3 diffuse, float metallic, float roughness) {
	vec3 h = normalize(v + l);
	float LdotH = max(dot(l, h), 0.0);
	float NdotH = max(dot(n, h), 0.0);
	float VdotH = max(dot(v, h), 0.0);
	float LdotN = max(dot(l, n), 0.0);
	float VdotN = max(dot(v, n), 0.0);
	vec3 brdf = brdf(LdotH, NdotH, VdotH, LdotN, VdotN, diffuse, metallic, roughness);
	vec3 ret = lc * brdf * LdotN;

	return ret;
}

float shadowsValue(float bias) {
	vec3 proj = fragLightSpace.xyz / fragLightSpace.w;
	if (proj.z > 1.0) {
		return 0.0;
	}
	proj = proj * 0.5 + 0.5;
	float curr = proj.z;
	float shadows = 0.0;
	
	vec2 texelSize = 1.0 / textureSize(shadowsTexSampler, 0);
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float pcf = texture(shadowsTexSampler, proj.xy + vec2(x, y) * texelSize).x;
			shadows += curr - bias > pcf ? 1.0 : 0.0;
		}
	}

	return shadows / 9.0;
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
	float shadows = 0.0;
	for (int i = 0; i < lights.fragNumDirLights; i++) {
		l = normalize(-lights.fragDirLights[i]);
		color += shade(n, v, l, lights.fragDirLightsColor[i], d, metallic, roughness);
		float bias = max(0.0005 * (1.0 - dot(n, l)), 0.00005);
		shadows += shadowsValue(bias);
	}
	for (int i = 0; i < lights.fragNumPointLights; i++) {
		l = normalize(lights.fragPointLights[i] - fragPos);
		float distance = length(lights.fragPointLights[i] - fragPos);
		float attenuation = 1.0 / pow(distance, 2.2);
		vec3 radiance = lights.fragPointLightsColor[i] * attenuation;
		color += shade(n, v, l, radiance, d, metallic, roughness);
	}
	color *= (1.0 - shadows);
	vec3 ambient = vec3(0.03) * d * ao;
	color += ambient;

	// HDR
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));
	outColor = vec4(color, diffuse.w);
}