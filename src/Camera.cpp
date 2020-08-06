#include "Camera.h"

Camera::Camera() {
	posX = -3.0f;
	posY = 0.3f;
	posZ = 0.0f;
	frontX = 1.0f;
	frontY = 0.0f;
	frontZ = 0.0f;
	upX = 0.0f;
	upY = 1.0f;
	upZ = 0.0f;
	movementSpeed = 2.5;
	frameEvent = nullptr;
}

Camera::Camera(float xPos, float yPos, float zPos) {
	posX = xPos;
	posY = yPos;
	posZ = zPos;
	frontX = 1.0f;
	frontY = 0.0f;
	frontZ = 0.0f;
	upX = 0.0f;
	upY = 1.0f;
	upZ = 0.0f;
	movementSpeed = 2.5;
	frameEvent = nullptr;
}

Camera::Camera(float xPos, float yPos, float zPos, float xFront, float yFront, float zFront) {
	posX = xPos;
	posY = yPos;
	posZ = zPos;
	frontX = xFront;
	frontY = yFront;
	frontZ = zFront;
	upX = 0.0f;
	upY = 1.0f;
	upZ = 0.0f;
	movementSpeed = 2.5;
	frameEvent = nullptr;
}

Camera::Camera(float xPos, float yPos, float zPos, float xFront, float yFront, float zFront, float ms) {
	posX = xPos;
	posY = yPos;
	posZ = zPos;
	frontX = xFront;
	frontY = yFront;
	frontZ = zFront;
	upX = 0.0f;
	upY = 1.0f;
	upZ = 0.0f;
	movementSpeed = ms;
	frameEvent = nullptr;
}

float Camera::getPositionX() {
	return posX;
}

float Camera::getPositionY() {
	return posY;
}

float Camera::getPositionZ() {
	return posZ;
}

void Camera::setPosition(float x, float y, float z) {
	posX = x;
	posY = y;
	posZ = z;
}

float Camera::getFrontX() {
	return frontX;
}

float Camera::getFrontY() {
	return frontY;
}

float Camera::getFrontZ() {
	return frontZ;
}

void Camera::setFront(float x, float y, float z) {
	frontX = x;
	frontY = y;
	frontZ = z;
}

float Camera::getUpX() {
	return upX;
}

float Camera::getUpY() {
	return upY;
}

float Camera::getUpZ() {
	return upZ;
}

float Camera::getMovementSpeed() {
	return movementSpeed;
}

void Camera::setMovementSpeed(float newMS) {
	movementSpeed = newMS;
}
