#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(float x, float y, float z, float r, float g, float b) {
	direction = glm::vec3(x, y, z);
	color = glm::vec3(r, g, b);
}

float DirectionalLight::getDirectionX() {
	return direction.x;
}

float DirectionalLight::getDirectionY() {
	return direction.y;
}

float DirectionalLight::getDirectionZ() {
	return direction.z;
}

void DirectionalLight::setDirection(float x, float y, float z) {
	direction = glm::vec3(x, y, z);
}

float DirectionalLight::getColorR() {
	return color.x;
}

float DirectionalLight::getColorG() {
	return color.y;
}

float DirectionalLight::getColorB() {
	return color.z;
}

void DirectionalLight::setColor(float r, float g, float b) {
	color = glm::vec3(r, g, b);
}