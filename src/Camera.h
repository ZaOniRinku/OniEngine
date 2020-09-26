#pragma once
#include "../external/glfw/include/GLFW/glfw3.h"

class Camera {
public:
	Camera();
	Camera(float xPos, float yPos, float zPos);
	Camera(float xPos, float yPos, float zPos, float xFront, float yFront, float zFront);
	Camera(float xPos, float yPos, float zPos, float xFront, float yFront, float zFront, float ms);
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
	float posX;
	float posY;
	float posZ;

	float frontX;
	float frontY;
	float frontZ;

	float upX;
	float upY;
	float upZ;

	float movementSpeed;
};
