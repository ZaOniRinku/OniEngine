#include "Object.h"

Object::Object(float x, float y, float z, float mScale) {
	posX = x;
	posY = y;
	posZ = z;
	scale = mScale;
	rotX = 0.0f;
	rotY = 0.0f;
	rotZ = 0.0f;
	node = nullptr;
	frameEvent = nullptr;
}

Object::Object(float x, float y, float z, float mScale, float xRot, float yRot, float zRot) {
	posX = x;
	posY = y;
	posZ = z;
	scale = mScale;
	rotX = xRot;
	rotY = yRot;
	rotZ = zRot;
	node = nullptr;
	frameEvent = nullptr;
}

void Object::move(float x, float y, float z) {
	if (node) {
		for (SGNode* child : node->getChildren()) {
			child->getObject()->move(x, y, z);
		}
	}
	posX = posX + x;
	posY = posY + y;
	posZ = posZ + z;
}

float Object::getPositionX() {
	return posX;
}

float Object::getPositionY() {
	return posY;
}

float Object::getPositionZ() {
	return posZ;
}

void Object::setPosition(float newX, float newY, float newZ) {
	posX = newX;
	posY = newY;
	posZ = newZ;
}

float Object::getScale() {
	return scale;
}

float Object::getRotationX() {
	return rotX;
}

float Object::getRotationY() {
	return rotY;
}

float Object::getRotationZ() {
	return rotZ;
}

void Object::setRotation(float newRotX, float newRotY, float newRotZ) {
	rotX = newRotX;
	rotY = newRotY;
	rotZ = newRotZ;
}

SGNode* Object::getNode() {
	return node;
}

void Object::setNode(SGNode* newNode) {
	node = newNode;
}

Mesh* Object::getMesh() {
	return mesh;
}

void Object::setMesh(Mesh *newMesh) {
	mesh = newMesh;
}

Material* Object::getMaterial() {
	return material;
}

void Object::setMaterial(Material* newMaterial) {
	material = newMaterial;
}

std::string Object::getName() {
	return name;
}

void Object::setName(std::string newName) {
	name = newName;
}

std::vector<VkDescriptorSet>* Object::getDescriptorSets() {
	return &descriptorSets;
}

std::vector<VkDescriptorSet>* Object::getShadowsDescriptorSets() {
	return &shadowsDescriptorSets;
}

std::vector<VkBuffer>* Object::getObjectBuffers() {
	return &objectBuffers;
}

std::vector<VkDeviceMemory>* Object::getObjectBufferMemories() {
	return &objectBufferMemories;
}

VkPipeline* Object::getGraphicsPipeline() {
	return graphicsPipeline;
}

void Object::setGraphicsPipeline(VkPipeline* newGraphicsPipeline) {
	graphicsPipeline = newGraphicsPipeline;
}