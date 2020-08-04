#include "Camera.h"

Camera::Camera() {
	pos = glm::vec3(-3.0f, 0.3f, 0.0f);
	front = glm::vec3(1.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	movementSpeed = 2.5;
	frameEvent = nullptr;
}

Camera::Camera(float posX, float posY, float posZ) {
	pos = glm::vec3(posX, posY, posZ);
	front = glm::vec3(1.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	movementSpeed = 2.5;
	frameEvent = nullptr;
}

Camera::Camera(float posX, float posY, float posZ, float frontX, float frontY, float frontZ) {
	pos = glm::vec3(posX, posY, posZ);
	front = glm::vec3(frontX, frontY, frontZ);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	movementSpeed = 2.5;
	frameEvent = nullptr;
}

Camera::Camera(float posX, float posY, float posZ, float frontX, float frontY, float frontZ, float ms) {
	pos = glm::vec3(posX, posY, posZ);
	front = glm::vec3(frontX, frontY, frontZ);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	movementSpeed = ms;
	frameEvent = nullptr;
}

float Camera::getPositionX() {
	return pos.x;
}

float Camera::getPositionY() {
	return pos.y;
}

float Camera::getPositionZ() {
	return pos.z;
}

void Camera::setPosition(float x, float y, float z) {
	pos = { x, y, z };
}

float Camera::getFrontX() {
	return front.x;
}

float Camera::getFrontY() {
	return front.y;
}

float Camera::getFrontZ() {
	return front.z;
}

void Camera::setFront(float x, float y, float z) {
	front = { x, y, z };
}

float Camera::getUpX() {
	return up.x;
}

float Camera::getUpY() {
	return up.y;
}

float Camera::getUpZ() {
	return up.z;
}

float Camera::getMovementSpeed() {
	return movementSpeed;
}

void Camera::setMovementSpeed(float newMS) {
	movementSpeed = newMS;
}
