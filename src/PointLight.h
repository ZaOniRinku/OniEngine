#pragma once
#include "../external/glfw/include/GLFW/glfw3.h"
#include "../external/glm/glm.hpp"

class PointLight {
public:
	PointLight(float x, float y, float z, float r, float g, float b);
	float getPositionX();
	float getPositionY();
	float getPositionZ();
	void setPosition(float x, float y, float z);
	float getColorR();
	float getColorG();
	float getColorB();
	void setColor(float r, float g, float b);

	void(*frameEvent)(PointLight *pointLight, GLFWwindow* window, double deltaTime);
private:
	glm::vec3 position;
	glm::vec3 color;
};