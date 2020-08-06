#pragma once
#include "../external/glfw/include/GLFW/glfw3.h"
#include "../external/glm/glm.hpp"

class SpotLight {
public:
	SpotLight(float px, float py, float pz, float dx, float dy, float dz, float r, float g, float b, float co, float oCo);
	float getPositionX();
	float getPositionY();
	float getPositionZ();
	void setPosition(float x, float y, float z);
	float getDirectionX();
	float getDirectionY();
	float getDirectionZ();
	void setDirection(float x, float y, float z);
	float getColorR();
	float getColorG();
	float getColorB();
	void setColor(float r, float g, float b);
	float getCutoff();
	void setCutoff(float newCutoff);
	float getOutCutoff();
	void setOutCutoff(float newOutCutoff);
	bool isTorchlight();
	void actAsTorchlight(bool torchlightVal);

	void(*frameEvent)(SpotLight* spotLight, GLFWwindow* window, double deltaTime);
private:
	alignas(glm::vec4) glm::vec3 position;
	alignas(glm::vec4) glm::vec3 direction;
	alignas(glm::vec4) glm::vec3 color;
	float cutoff;
	float outCutoff;

	bool torchlight;
};

