#include "Scene.h"

Scene::Scene() {
	sceneRoot = SGNode();
	camera = Camera();
}

Scene::Scene(Camera sceneCamera) {
	sceneRoot = SGNode();
	camera = sceneCamera;
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

Object * Scene::getSkybox()
{
	return skybox;
}

void Scene::setSkybox(Object *newSkybox)
{
	skybox = newSkybox;
}

void Scene::viewSceneGraph() {
	std::cout << "Scene" << std::endl;
	sceneRoot.viewSceneNode(1);
}

int Scene::nbElements() {
	return sceneRoot.nbElements()+1;
}

std::vector<DirectionalLight*>* Scene::getDirectionalLights()
{
	return &dirLights;
}

void Scene::addDirectionalLight(DirectionalLight* newDirectionalLight)
{
	dirLights.push_back(newDirectionalLight);
}

std::vector<PointLight*>* Scene::getPointLights()
{
	return &pointLights;
}

void Scene::addPointLight(PointLight* newPointLight)
{
	pointLights.push_back(newPointLight);
	
}