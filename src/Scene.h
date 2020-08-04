#pragma once
#include "SGNode.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

struct LightsBufferObject {
	alignas(16) int numDirLights;
	alignas(16) glm::vec3 dirLights[10];
	alignas(16) glm::vec3 dirLightsColor[10];
	alignas(16) int numPointLights;
	alignas(16) glm::vec3 pointLights[10];
	alignas(16) glm::vec3 pointLightsColor[10];
	alignas(16) int numSpotLights;
	alignas(16) glm::vec3 spotLightsPos[10];
	alignas(16) glm::vec3 spotLightsDir[10];
	alignas(16) glm::vec3 spotLightsColor[10];
	alignas(16) float spotLightsCutoff[10];
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

	// Lights
	std::vector<DirectionalLight*> dirLights;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	LightsBufferObject lbo;

	// Dummy lights
	DirectionalLight dummyDirLight = DirectionalLight(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
};
