#include "PointLight.h"

PointLight::PointLight(float x, float y, float z, float r, float g, float b) {
	positionX = x;
	positionY = y;
	positionZ = z;
	colorR = r;
	colorG = g;
	colorB = b;
	frameEvent = nullptr;
}

float PointLight::getPositionX() {
	return positionX;
}

float PointLight::getPositionY() {
	return positionY;
}

float PointLight::getPositionZ() {
	return positionZ;
}

void PointLight::setPosition(float x, float y, float z) {
	positionX = x;
	positionY = y;
	positionZ = z;
}

float PointLight::getColorR() {
	return colorR;
}

float PointLight::getColorG() {
	return colorG;
}

float PointLight::getColorB() {
	return colorB;
}

void PointLight::setColor(float r, float g, float b) {
	colorR = r;
	colorG = g;
	colorB = b;
}