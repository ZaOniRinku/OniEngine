#pragma once
#include "SGNode.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"

struct LightsBufferObject {
	alignas(16) int numDirLights;
	alignas(16) glm::vec3 dirLights[10];
	alignas(16) glm::vec3 dirLightsColor[10];
	alignas(16) int numPointLights;
	alignas(16) glm::vec3 pointLights[10];
	alignas(16) glm::vec3 pointLightsColor[10];
};

class Scene {
public:
	Scene();
	Scene(Camera sceneCamera);
	SGNode* getRoot();
	void setRoot(SGNode newRoot);
	Camera* getCamera();
	void setCamera(Camera newCamera);
	void viewSceneGraph();
	int nbElements();
	std::vector<DirectionalLight*>* getDirectionalLights();
	void addDirectionalLight(DirectionalLight* newDirectionalLight);
	std::vector<PointLight*>* getPointLights();
	void addPointLight(PointLight* newPointLight);
private:
	SGNode sceneRoot;
	Camera camera;

	// Lights
	std::vector<DirectionalLight*> dirLights;
	std::vector<PointLight*> pointLights;

	LightsBufferObject lbo;
};
