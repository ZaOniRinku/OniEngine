#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(float x, float y, float z, float r, float g, float b) {
	directionX = x;
	directionY = y;
	directionZ = z;
	colorR = r;
	colorG = g;
	colorB = b;
	frameEvent = nullptr;
}

float DirectionalLight::getDirectionX() {
	return directionX;
}

float DirectionalLight::getDirectionY() {
	return directionY;
}

float DirectionalLight::getDirectionZ() {
	return directionZ;
}

void DirectionalLight::setDirection(float x, float y, float z) {
	directionX = x;
	directionY = y;
	directionZ = z;
}

float DirectionalLight::getColorR() {
	return colorR;
}

float DirectionalLight::getColorG() {
	return colorG;
}

float DirectionalLight::getColorB() {
	return colorB;
}

void DirectionalLight::setColor(float r, float g, float b) {
	colorR = r;
	colorG = g;
	colorB = b;
}