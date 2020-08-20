#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

layout(constant_id = 0) const int numShadowmaps = MAX_DIR_LIGHTS + MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS;

layout(binding = 2) uniform Lights {
	vec3 numLights;
	vec3 dirLightsDir[MAX_DIR_LIGHTS];
	vec3 dirLightsColor[MAX_DIR_LIGHTS];
	vec3 pointLightsPos[MAX_POINT_LIGHTS];
	vec3 pointLightsColor[MAX_POINT_LIGHTS];
	vec3 spotLightsPos[MAX_SPOT_LIGHTS];
	vec3 spotLightsDir[MAX_SPOT_LIGHTS];
	vec3 spotLightsColor[MAX_SPOT_LIGHTS];
	vec2 spotLightsCutoffs[MAX_SPOT_LIGHTS];
} lights;

layout(binding = 4) uniform sampler2D shadowsTexSampler[numShadowmaps];
layout(binding = 5) uniform sampler2D diffuseTexSampler;
layout(binding = 6) uniform sampler2D normalTexSampler;
layout(binding = 7) uniform sampler2D metallicTexSampler;
layout(binding = 8) uniform sampler2D roughnessTexSampler;
layout(binding = 9) uniform sampler2D AOTexSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoords;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 fragCamPos;
layout(location = 4) in vec4 fragDirLightsSpace[MAX_DIR_LIGHTS];
layout(location = MAX_DIR_LIGHTS + 4) in vec4 fragSpotLightsSpace[MAX_SPOT_LIGHTS];
layout(location = MAX_SPOT_LIGHTS + MAX_DIR_LIGHTS + 4) in mat3 fragTBN;

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

vec3 diffuseFresnelCorrection(vec3 ior) {
	vec3 iorsquare = ior * ior;
	bvec3 TIR = lessThan(ior, vec3(1.0));
	vec3 invdenum = mix(vec3(1.0), vec3(1.0) / (iorsquare * iorsquare * (vec3(554.33) - 380.7 * ior)), TIR);
	vec3 num = ior * mix(vec3(0.1921156102251088), ior * 298.25 - 261.38 * iorsquare + 138.43, TIR);
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

float shadowsValue(vec4 lightSpace, int shadowmapIndex, float bias) {
	vec3 proj = lightSpace.xyz / lightSpace.w;
	if (proj.z > 1.0) {
		return 0.0;
	}
	proj = proj * 0.5 + 0.5;
	float curr = proj.z;
	float shadows = 0.0;
	
	vec2 texelSize = 1.0 / textureSize(shadowsTexSampler[shadowmapIndex], 0);
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float pcf = texture(shadowsTexSampler[shadowmapIndex], proj.xy + vec2(x, y) * texelSize).x;
			shadows += curr - bias > pcf ? 1.0 : 0.0;
		}
	}

	return shadows / 9.0;
}

void main() {
	vec4 diffuse = texture(diffuseTexSampler, fragTexCoords);
	vec3 normal = texture(normalTexSampler, fragTexCoords).xyz;
	float metallic = texture(metallicTexSampler, fragTexCoords).x;
	float roughness = texture(roughnessTexSampler, fragTexCoords).x;
	float ao = texture(AOTexSampler, fragTexCoords).x;

	vec3 d = vec3(diffuse);
	vec3 n = normal * 2.0 - 1.0;
	n = normalize(fragTBN * n);
	vec3 v = normalize(fragCamPos - fragPos);
	vec3 l;
	
	vec3 color = vec3(0.0);
	int numDirLights = int(lights.numLights.x);
	int numPointLights = int(lights.numLights.y);
	int numSpotLights = int(lights.numLights.z);
	int shadowmapIndex = 0;
	float shadows = 0.0;
	// Directional Lights
	for (int i = 0; i < numDirLights; i++) {
		l = normalize(-lights.dirLightsDir[i]);
		float bias = max(0.0005 * (1.0 - dot(n, l)), 0.00005);
		shadows = shadowsValue(fragDirLightsSpace[i] , shadowmapIndex, bias);
		color += shade(n, v, l, lights.dirLightsColor[i], d, metallic, roughness) * (1.0 - shadows);
		shadowmapIndex++;
	}
	// Point Lights
	for (int i = 0; i < numPointLights; i++) {
		l = normalize(lights.pointLightsPos[i] - fragPos);
		float distance = length(lights.pointLightsPos[i] - fragPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lights.pointLightsColor[i] * attenuation;
		color += shade(n, v, l, radiance, d, metallic, roughness);
	}
	// Spot Lights
	for (int i = 0; i < numSpotLights; i++) {
		l = normalize(lights.spotLightsPos[i] - fragPos);
		float theta = dot(l, normalize(-lights.spotLightsDir[i]));
		if (theta > lights.spotLightsCutoffs[i].x) {
			float bias = max(0.0005 * (1.0 - dot(n, l)), 0.00005);
			shadows = shadowsValue(fragSpotLightsSpace[i], shadowmapIndex, bias);
			color += shade(n, v, l, lights.spotLightsColor[i], d, metallic, roughness) * (1.0 - shadows);
		} else if (theta > lights.spotLightsCutoffs[i].y) {
			float bias = max(0.0005 * (1.0 - dot(n, l)), 0.00005);
			shadows = shadowsValue(fragSpotLightsSpace[i], shadowmapIndex, bias);
			float epsilon = lights.spotLightsCutoffs[i].x - lights.spotLightsCutoffs[i].y;
			float intensity = clamp((theta - lights.spotLightsCutoffs[i].y) / epsilon, 0.0, 1.0);
			color += shade(n, v, l, lights.spotLightsColor[i] * intensity, d * intensity, metallic, roughness) * (1.0 - shadows);
		}
		shadowmapIndex++;
	}
	vec3 ambient = vec3(0.03) * d * ao;
	color += ambient;

	// HDR
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));
	outColor = vec4(color, diffuse.w);
}