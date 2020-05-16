#include "Object.h"

Object::Object() {}

Object::Object(std::string mPath, glm::vec3 oPos, float mScale) {
	modelPath = mPath;
	pos = oPos;
	scale = mScale;
	//loadModel();
	node = nullptr;
	rot = { 0.0f, 0.0f, 0.0f };
}

Object::Object(std::string mPath, float x, float y, float z, float mScale) {
	modelPath = mPath;
	pos = { x, y, z };
	scale = mScale;
	//loadModel();
	node = nullptr;
	rot = { 0.0f, 0.0f, 0.0f };
}

Object::Object(std::string mPath, float x, float y, float z, float mScale, float xRot, float yRot, float zRot) {
	modelPath = mPath;
	pos = { x, y, z };
	scale = mScale;
	//loadModel();
	node = nullptr;
	rot = { xRot, yRot, zRot };
}

void Object::move(glm::vec3 movePosition) {
	for (SGNode* child : node->getChildren()) {
		child->getObject()->move(movePosition);
	}
	pos = pos + movePosition;
}

void Object::move(float x, float y, float z) {
	for (SGNode* child : node->getChildren()) {
		child->getObject()->move(x, y, z);
	}
	pos = { pos.x + x, pos.y + y, pos.z + z };
}

void Object::rescale(float newScale) {
	for (Vertex vertex : modelVertices) {
		vertex.pos = (vertex.pos / scale) * newScale;
	}
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

std::string Object::getModelPath() {
	return modelPath;
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

std::vector<Vertex>* Object::getModelVertices() {
	return &modelVertices;
}

std::vector<uint32_t>* Object::getModelIndices() {
	return &modelIndices;
}

VkBuffer* Object::getVertexBuffer() {
	return &vertexBuffer;
}

VkDeviceMemory* Object::getVertexBufferMemory() {
	return &vertexBufferMemory;
}

VkBuffer* Object::getIndexBuffer() {
	return &indexBuffer;
}

VkDeviceMemory* Object::getIndexBufferMemory() {
	return &indexBufferMemory;
}

VkDescriptorSet* Object::getDescriptorSet(int index) {
	return descriptorSets[index];
}

void Object::addDescriptorSet(VkDescriptorSet* newDescriptorSet) {
	descriptorSets.push_back(newDescriptorSet);
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