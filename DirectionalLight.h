#pragma once
#include <glm/glm.hpp>

class DirectionalLight {
public:
	DirectionalLight(float x, float y, float z, float r, float g, float b);
	float getDirectionX();
	float getDirectionY();
	float getDirectionZ();
	void setDirection(float x, float y, float z);
	float getColorR();
	float getColorG();
	float getColorB();
	void setColor(float r, float g, float b);
private:
	glm::vec3 direction;
	glm::vec3 color;
};

