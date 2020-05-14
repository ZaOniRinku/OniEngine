#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D diffuseTexSampler;
layout(binding = 2) uniform sampler2D normalTexSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragEye;
layout(location = 2) in vec3 fragLightDir;
layout(location = 3) in vec2 fragTexCoord;
layout(location = 4) in vec3 fragAmbient;
layout(location = 5) in float fragDistance;

layout(location = 0) out vec4 outColor;

#define M_PI 3.1415926535897932384626433832795

float RDM_chiplus(float c) {
	return (c > 0.f) ? 1.f : 0.f;
}

float RDM_Beckmann(float NdotH, float alpha) {
	float xplus = RDM_chiplus(NdotH);
	float coscarretheta = pow(NdotH, 2);
	float d = xplus * (exp(-((1.f-coscarretheta)/coscarretheta)/pow(alpha,2))/(M_PI*pow(alpha, 2)*pow(NdotH, 4)));
	return d;
}

float RDM_Fresnel(float LdotH, float extIOR, float intIOR) {
	float f;
	float rs;
	float rp;
	float costhetai = LdotH;
	float coscarrethetai = pow(LdotH, 2);
	float sincarrethetat = pow(extIOR/intIOR, 2) * (1.f-coscarrethetai);
	float costhetat;

	if (sincarrethetat > 1.f) {
		return 1.f;
	} else {
		costhetat = sqrt(1-sincarrethetat);
		rs = pow(extIOR*costhetai-intIOR*costhetat, 2)/pow(extIOR*costhetai+intIOR*costhetat, 2);
		rp = pow(extIOR*costhetat-intIOR*costhetai, 2)/pow(extIOR*costhetat+intIOR*costhetai, 2);
		f = (1.f/2.f)*(rs+rp);
		if (f > 1.f) {
			return 1.f;
		}
	}

	return f;
}

float RDM_G1(float DdotH, float DdotN, float alpha) {
	float k = DdotH/DdotN;
	float costheta = DdotN;
	float tantheta = sqrt((1.f-pow(costheta, 2))/costheta);
	float b = 1.f/(alpha*tantheta);
	float xplus = RDM_chiplus(k);

	if (b < 1.6) {
		return xplus*((3.535*b+(2.181*pow(b, 2)))/(1.f+2.276*b+2.577*pow(b, 2)));
	}

	return xplus;
}

float RDM_Smith(float LdotH, float LdotN, float VdotH, float VdotN, float alpha) {
	float g1l = RDM_G1(LdotH, LdotN, alpha);
	float g1v = RDM_G1(VdotH, VdotN, alpha);

	return g1l*g1v;
}

vec3 RDM_bsdf_s(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN) {
	float d = RDM_Beckmann(NdotH, 0.5);
	float f = RDM_Fresnel(LdotH, 1.f, 1.329);
	float g = RDM_Smith(LdotH, LdotN, VdotH, VdotN, 0.5);

	return vec3(255, 255, 255)*((d*f*g)/(4.f*LdotN*VdotN));
}

vec3 RDM_bsdf_d() {
	vec4 diffuse = texture(diffuseTexSampler, fragTexCoord);
	return vec3(normalize(diffuse))/M_PI;
}

vec3 RDM_bsdf(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN) {
	vec3 specular = RDM_bsdf_s(LdotH, NdotH, VdotH, LdotN, VdotN);
	vec3 diffuse = RDM_bsdf_d();
	return diffuse+specular;
}

vec3 shade(vec3 n, vec3 v, vec3 l, vec3 lc) {
	vec3 h = normalize(v+l);
	float LdotH = dot(l,h);
	float NdotH = dot(n,h);
	float VdotH = dot(v,h);
	float LdotN = dot(l,n);
	float VdotN = dot(v,n);
	vec3 bsdf = RDM_bsdf(LdotH, NdotH, VdotH, LdotN, VdotN);
	vec3 ret = lc*bsdf*LdotN;

	return ret;
}

void main() {
	vec4 normal = texture(normalTexSampler, fragTexCoord);
	vec3 n = vec3(normalize(normal * 2.0 - 1.0));
	vec3 e = normalize(fragEye);
	vec3 l = normalize(fragLightDir);
	vec4 diffuse = texture(diffuseTexSampler, fragTexCoord);

	if (fragDistance > 10.0) {
		outColor =  vec4(fragAmbient, 0.0) * diffuse;
	} else {
		outColor = vec4(shade(n, e, l, vec3(1.f)), 0.0);
	}

  /*
  vec3 n = normalize(fragNormal);
  vec3 e = normalize(fragEye);
  vec3 l = normalize(fragLightDir);
	
  vec4 normal = texture(normalTexSampler, fragTexCoord);
  n = vec3(normalize(normal * 2.0 - 1.0));
  vec4 diffuse = texture(diffuseTexSampler, fragTexCoord);
  float intensity = max(dot(n, l), 0.0);
  if (fragDistance > 10.0) {
    outColor = vec4(fragAmbient, 0.0) * diffuse;
  }
  else {
    outColor = max(intensity * diffuse, vec4(fragAmbient, 0.0) * diffuse);
  }*/
}
