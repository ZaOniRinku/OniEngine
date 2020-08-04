#include "PointLight.h"

PointLight::PointLight(float x, float y, float z, float r, float g, float b) {
	position = glm::vec3(x, y, z);
	color = glm::vec3(r, g, b);
	frameEvent = nullptr;
}

float PointLight::getPositionX() {
	return position.x;
}

float PointLight::getPositionY() {
	return position.y;
}

float PointLight::getPositionZ() {
	return position.z;
}

void PointLight::setPosition(float x, float y, float z) {
	position = glm::vec3(x, y, z);
}

float PointLight::getColorR() {
	return color.x;
}

float PointLight::getColorG() {
	return color.y;
}

float PointLight::getColorB() {
	return color.z;
}

void PointLight::setColor(float r, float g, float b) {
	color = glm::vec3(r, g, b);
}