#include "Scene.h"

Scene::Scene() {
	dirLights.push_back(&dummyDirLight);
}

Scene::Scene(Camera sceneCamera) {
	camera = sceneCamera;
}

Scene::Scene(Camera sceneCamera, Skybox sceneSkybox) {
	camera = sceneCamera;
	skybox = sceneSkybox;
}

SGNode* Scene::getRoot() {
	return &sceneRoot;
}

void Scene::setRoot(SGNode newRoot) {
	sceneRoot = newRoot;
}

Camera* Scene::getCamera() {
	return &camera;
}

void Scene::setCamera(Camera newCamera) {
	camera = newCamera;
}

Skybox* Scene::getSkybox() {
	return &skybox;
}

void Scene::setSkybox(Skybox newSkybox) {
	skybox = newSkybox;
}

void Scene::viewSceneGraph() {
	std::cout << "Scene" << std::endl;
	sceneRoot.viewSceneNode(1);
}

int Scene::nbElements() {
	return (int)elements.size();
}

std::vector<Object*>& Scene::getElements() {
	return elements;
}

std::vector<Object*>& Scene::getElementsFE() {
	return elementsFE;
}

void Scene::flattenSG() {
	sceneRoot.flatten(&sceneRoot, &elements);
	sceneRoot.flattenFrameEvent(&sceneRoot, &elementsFE);
}

std::vector<DirectionalLight*>& Scene::getDirectionalLights() {
	return dirLights;
}

void Scene::addDirectionalLight(DirectionalLight* newDirectionalLight) {
	if (dirLights.size() == 1 && dirLights[0] == &dummyDirLight) {
		dirLights[0] = newDirectionalLight;
	} else {
		dirLights.push_back(newDirectionalLight);
	}
}

std::vector<PointLight*>& Scene::getPointLights() {
	return pointLights;
}

void Scene::addPointLight(PointLight* newPointLight) {
	if (pointLights.size() == 1 && pointLights[0] == &dummyPointLight) {
		pointLights[0] = newPointLight;
	}
	else {
		pointLights.push_back(newPointLight);
	}
}

std::vector<SpotLight*>& Scene::getSpotLights() {
	return spotLights;
}

void Scene::addSpotLight(SpotLight* newSpotLight) {
	if (spotLights.size() == 1 && spotLights[0] == &dummySpotLight) {
		spotLights[0] = newSpotLight;
	}
	else {
		spotLights.push_back(newSpotLight);
	}
}