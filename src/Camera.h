#pragma once
#include "../external/glfw/include/GLFW/glfw3.h"
#include "../external/glm/glm.hpp"

class Camera {
public:
	Camera();
	Camera(float posX, float posY, float posZ);
	Camera(float posX, float posY, float posZ, float frontX, float frontY, float frontZ);
	Camera(float posX, float posY, float posZ, float frontX, float frontY, float frontZ, float ms);
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
	void(*frameEvent)(Camera *obj, GLFWwindow* window, double deltaTime);
private:
	alignas(glm::vec4) glm::vec3 pos;
	alignas(glm::vec4) glm::vec3 front;
	alignas(glm::vec4) glm::vec3 up;
	float movementSpeed;
};
