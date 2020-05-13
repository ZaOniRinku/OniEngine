#include "Scene.h"

Scene::Scene() {
	sceneRoot = SGNode();
	camera = Camera();
	ambientLightValue = 1.0f;
	ambientLightColor = { 0.0f, 0.0f, 1.0f };
	lightPosition = { 0.0f, 0.0f, 0.0f };
}

Scene::Scene(Camera sceneCamera) {
	sceneRoot = SGNode();
	camera = sceneCamera;
	ambientLightValue = 1.0f;
	ambientLightColor = { 1.0f, 1.0f, 1.0f };
	lightPosition = { 0.0f, 0.0f, 0.0f };
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

float Scene::getAmbientLightValue() {
	return ambientLightValue;
}

void Scene::setAmbientLightValue(float newAmbientLightValue) {
	ambientLightValue = newAmbientLightValue;
}

float Scene::getAmbientLightColorR() {
	return ambientLightColor.x;
}

float Scene::getAmbientLightColorG() {
	return ambientLightColor.y;
}

float Scene::getAmbientLightColorB() {
	return ambientLightColor.z;
}

void Scene::setAmbientLightColor(float newR, float newG, float newB) {
	ambientLightColor = { newR, newG, newB };
}

float Scene::getLightPositionX() {
	return lightPosition.x;
}

float Scene::getLightPositionY() {
	return lightPosition.y;
}

float Scene::getLightPositionZ() {
	return lightPosition.z;
}

void Scene::setLightPosition(float newX, float newY, float newZ) {
	lightPosition = { newX, newY, newZ };
}
