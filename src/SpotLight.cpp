#include "SpotLight.h"

SpotLight::SpotLight(float px, float py, float pz, float dx, float dy, float dz, float r, float g, float b, float co, float oCo) {
	positionX = px;
	positionY = py;
	positionZ = pz;
	directionX = dx;
	directionY = dy;
	directionZ = dz;
	colorR = r;
	colorG = g;
	colorB = b;
	cutoff = glm::cos(glm::radians(co));
	outCutoff = glm::cos(glm::radians(oCo));
	torchlight = false;
	frameEvent = nullptr;
}

float SpotLight::getPositionX() {
	return positionX;
}

float SpotLight::getPositionY() {
	return positionY;
}

float SpotLight::getPositionZ() {
	return positionZ;
}

void SpotLight::setPosition(float x, float y, float z) {
	positionX = x;
	positionY = y;
	positionZ = z;
}

float SpotLight::getDirectionX() {
	return directionX;
}

float SpotLight::getDirectionY() {
	return directionY;
}

float SpotLight::getDirectionZ() {
	return directionZ;
}

void SpotLight::setDirection(float x, float y, float z) {
	directionX = x;
	directionY = y;
	directionZ = z;
}

float SpotLight::getColorR() {
	return colorR;
}

float SpotLight::getColorG() {
	return colorG;
}

float SpotLight::getColorB() {
	return colorB;
}

void SpotLight::setColor(float r, float g, float b) {
	colorR = r;
	colorG = g;
	colorB = b;
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
}