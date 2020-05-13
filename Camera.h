#pragma once
#include <glm/glm.hpp>

class Camera {
public:
	Camera();
	Camera(glm::vec3 cPos);
	Camera(glm::vec3 cPos, glm::vec3 cFront);
	Camera(glm::vec3 cPos, glm::vec3 cFront, float ms);
	float getPositionX();
	float getPositionY();
	float getPositionZ();
	void setPosition(float x, float y, float z);
	float getFrontX();
	float getFrontY();
	float getFrontZ();
	void setFront(float x, float y, float z);
	float getUpX();
	float getUpY();
	float getUpZ();
	float getMovementSpeed();
	void setMovementSpeed(float newMS);
private:
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	float movementSpeed;
};
