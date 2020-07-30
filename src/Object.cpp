#include "Object.h"

Object::Object() {}

Object::Object(float x, float y, float z, float mScale) {
	pos = { x, y, z };
	scale = mScale;
	node = nullptr;
	rot = { 0.0f, 0.0f, 0.0f };
}

Object::Object(float x, float y, float z, float mScale, float xRot, float yRot, float zRot) {
	pos = { x, y, z };
	scale = mScale;
	node = nullptr;
	rot = { xRot, yRot, zRot };
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

VkDescriptorSet* Object::getDescriptorSet(int index) {
	return descriptorSets[index];
}

void Object::addDescriptorSet(VkDescriptorSet* newDescriptorSet) {
	descriptorSets.push_back(newDescriptorSet);
}

VkDescriptorSet* Object::getShadowsDescriptorSet(int index)
{
	return shadowsDescriptorSets[index];
}

void Object::addShadowsDescriptorSet(VkDescriptorSet* newShadowsDescriptorSet)
{
	shadowsDescriptorSets.push_back(newShadowsDescriptorSet);
}

VkBuffer* Object::getUniformBuffer(int index) {
	return uniformBuffers[index];
}

void Object::addUniformBuffer(VkBuffer* newUniformBuffer) {
	uniformBuffers.push_back(newUniformBuffer);
}

VkDeviceMemory* Object::getUniformBufferMemory(int index) {
	return uniformBuffersMemory[index];
}

void Object::addUniformBufferMemory(VkDeviceMemory* newUniformBufferMemory) {
	uniformBuffersMemory.push_back(newUniformBufferMemory);
}

VkPipeline* Object::getGraphicsPipeline() {
	return graphicsPipeline;
}

void Object::setGraphicsPipeline(VkPipeline* newGraphicsPipeline) {
	graphicsPipeline = newGraphicsPipeline;
}