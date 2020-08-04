#include "SpotLight.h"

SpotLight::SpotLight(float px, float py, float pz, float dx, float dy, float dz, float r, float g, float b, float co, float oCo) {
	position = glm::vec3(px, py, pz);
	direction = glm::vec3(dx, dy, dz);
	color = glm::vec3(r, g, b);
	cutoff = glm::cos(glm::radians(co));
	outCutoff = glm::cos(glm::radians(oCo));
	torchlight = false;
	frameEvent = nullptr;
}

float SpotLight::getPositionX() {
	return position.x;
}

float SpotLight::getPositionY() {
	return position.y;
}

float SpotLight::getPositionZ() {
	return position.z;
}

void SpotLight::setPosition(float x, float y, float z) {
	position = glm::vec3(x, y, z);
}

float SpotLight::getDirectionX() {
	return direction.x;
}

float SpotLight::getDirectionY() {
	return direction.y;
}

float SpotLight::getDirectionZ() {
	return direction.z;
}

void SpotLight::setDirection(float x, float y, float z) {
	direction = glm::vec3(x, y, z);
}

float SpotLight::getColorR() {
	return color.x;
}

float SpotLight::getColorG() {
	return color.y;
}

float SpotLight::getColorB() {
	return color.z;
}

void SpotLight::setColor(float r, float g, float b) {
	color = glm::vec3(r, g, b);
}

float SpotLight::getCutoff() {
	return cutoff;
}

void SpotLight::setCutoff(float newCutoff) {
	cutoff = glm::cos(glm::radians(newCutoff));
}

float SpotLight::getOutCutoff() {
	return outCutoff;
}

void SpotLight::setOutCutoff(float newOutCutoff) {
	outCutoff = glm::cos(glm::radians(newOutCutoff));
}

bool SpotLight::isTorchlight() {
	return torchlight;
}

void SpotLight::actAsTorchlight(bool torchlightVal) {
	torchlight = torchlightVal;
	direction = -direction;
}