#pragma once
#include <iostream>
#include "SGNode.h"
#include "Camera.h"

class Scene {
public:
	Scene();
	Scene(Camera sceneCamera);
	SGNode* getRoot();
	void setRoot(SGNode newRoot);
	Camera* getCamera();
	void setCamera(Camera newCamera);
	Object* getSkybox();
	void setSkybox(Object *newSkybox);
	void viewSceneGraph();
	int nbElements();
	float getAmbientLightValue();
	void setAmbientLightValue(float newAmbientLightValue);
	float getAmbientLightColorR();
	float getAmbientLightColorG();
	float getAmbientLightColorB();
	void setAmbientLightColor(float newR, float newG, float newB);
	float getLightPositionX();
	float getLightPositionY();
	float getLightPositionZ();
	void setLightPosition(float newX, float newY, float newZ);
private:
	SGNode sceneRoot;
	Camera camera;
	Object *skybox;

	// Ambient Light
	float ambientLightValue;
	glm::vec3 ambientLightColor;

	// Directional Light
	glm::vec3 lightPosition;
};
