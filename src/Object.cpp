#include "Object.h"

Object::Object(float x, float y, float z, float mScale) {
	pos = { x, y, z };
	scale = mScale;
	rot = { 0.0f, 0.0f, 0.0f };
	node = nullptr;
	frameEvent = nullptr;
}

Object::Object(float x, float y, float z, float mScale, float xRot, float yRot, float zRot) {
	pos = { x, y, z };
	scale = mScale;
	rot = { xRot, yRot, zRot };
	node = nullptr;
	frameEvent = nullptr;
}

void Object::move(float x, float y, float z) {
	if (node) {
		for (SGNode* child : node->getChildren()) {
			child->getObject()->move(x, y, z);
		}
	}
	pos = { pos.x + x, pos.y + y, pos.z + z };
}

float Object::getPositionX() {
	return pos.x;
}

float Object::getPositionY() {
	return pos.y;
}

float Object::getPositionZ() {
	return pos.z;
}

void Object::setPosition(float newX, float newY, float newZ) {
	pos = { newX, newY, newZ };
}

float Object::getScale() {
	return scale;
}

float Object::getRotationX() {
	return rot.x;
}

float Object::getRotationY() {
	return rot.y;
}

float Object::getRotationZ() {
	return rot.z;
}

void Object::setRotation(float newRotX, float newRotY, float newRotZ) {
	rot = { newRotX, newRotY, newRotZ };
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