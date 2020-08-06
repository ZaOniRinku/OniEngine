#pragma once
#include "SGNode.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

struct LightsBufferObject {
	alignas(16) glm::vec3 numLights;
	alignas(16) glm::vec3 dirLightsDir[10];
	alignas(16) glm::vec3 dirLightsColor[10];
	alignas(16) glm::vec3 pointLightsPos[10];
	alignas(16) glm::vec3 pointLightsColor[10];
	alignas(16) glm::vec3 spotLightsPos[10];
	alignas(16) glm::vec3 spotLightsDir[10];
	alignas(16) glm::vec3 spotLightsColor[10];
	alignas(16) glm::vec2 spotLightsCutoffs[10];
};

class Scene {
public:
	Scene();
	Scene(Camera sceneCamera);
	SGNode* getRoot();
	void setRoot(SGNode newRoot);
	Camera* getCamera();
	void setCamera(Camera newCamera);
	Object* getSkybox();
	void setSkybox(Object* newSkybox);
	void viewSceneGraph();
	int nbElements();
	std::vector<Object*>& getElements();
	std::vector<Object*>& getElementsFE();
	void flattenSG();
	std::vector<DirectionalLight*>& getDirectionalLights();
	void addDirectionalLight(DirectionalLight* newDirectionalLight);
	std::vector<PointLight*>& getPointLights();
	void addPointLight(PointLight* newPointLight);
	std::vector<SpotLight*>& getSpotLights();
	void addSpotLight(SpotLight* newSpotLight);
private:
	SGNode sceneRoot;
	Camera camera;
	Object* skybox;

	std::vector<Object*> elements;
	std::vector<Object*> elementsFE;

	// Lights
	std::vector<DirectionalLight*> dirLights;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	// Dummy lights
	DirectionalLight dummyDirLight = DirectionalLight(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	PointLight dummyPointLight = PointLight(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	SpotLight dummySpotLight = SpotLight(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
};
