#include "Object.h"

Object::Object() {}

Object::Object(std::string mPath, std::string dtPath, glm::vec3 oPos, float mScale) {
	modelPath = mPath;
	diffuseTexturePath = dtPath;
	normalTexturePath = dtPath;
	pos = oPos;
	scale = mScale;
	//loadModel();
	node = nullptr;
	rot = { 0.0f, 0.0f, 0.0f };
}

Object::Object(std::string mPath, std::string dtPath, float x, float y, float z, float mScale) {
	modelPath = mPath;
	diffuseTexturePath = dtPath;
	normalTexturePath = dtPath;
	pos = { x, y, z };
	scale = mScale;
	//loadModel();
	node = nullptr;
	rot = { 0.0f, 0.0f, 0.0f };
}

Object::Object(std::string mPath, std::string dtPath, float x, float y, float z, float mScale, float xRot, float yRot, float zRot) {
	modelPath = mPath;
	diffuseTexturePath = dtPath;
	normalTexturePath = dtPath;
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

std::string Object::getDiffuseTexturePath() {
	return diffuseTexturePath;
}

void Object::setDiffuseTexturePath(std::string newDiffuseTexturePath)
{
	diffuseTexturePath = newDiffuseTexturePath;
}

std::string Object::getNormalTexturePath() {
	return normalTexturePath;
}

void Object::setNormalTexturePath(std::string newNormalTexturePath)
{
	normalTexturePath = newNormalTexturePath;
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

uint32_t Object::getDiffuseMipLevel() {
	return diffuseMipLevel;
}

void Object::setDiffuseMipLevel(uint32_t newDiffuseMipLevel) {
	normalMipLevel = newDiffuseMipLevel;
}

uint32_t Object::getNormalMipLevel() {
	return normalMipLevel;
}

void Object::setNormalMipLevel(uint32_t newNormalMipLevel) {
	normalMipLevel = newNormalMipLevel;
}

VkImage* Object::getDiffuseTextureImage() {
	return &diffuseTextureImage;
}

void Object::setDiffuseTextureImage(VkImage newDiffuseTextureImage) {
	diffuseTextureImage = newDiffuseTextureImage;
}

VkDeviceMemory* Object::getDiffuseTextureImageMemory() {
	return &diffuseTextureImageMemory;
}

void Object::setDiffuseTextureImageMemory(VkDeviceMemory newDiffuseTextureImageMemory) {
	diffuseTextureImageMemory = newDiffuseTextureImageMemory;
}

VkImageView* Object::getDiffuseTextureImageView() {
	return &diffuseTextureImageView;
}

void Object::setDiffuseTextureImageView(VkImageView newDiffuseTextureImageView) {
	diffuseTextureImageView = newDiffuseTextureImageView;
}

VkSampler* Object::getDiffuseTextureSampler() {
	return &diffuseTextureSampler;
}

void Object::setDiffuseTextureSampler(VkSampler newDiffuseTextureSampler) {
	diffuseTextureSampler = newDiffuseTextureSampler;
}

VkImage* Object::getNormalTextureImage() {
	return &normalTextureImage;
}

void Object::setNormalTextureImage(VkImage newNormalTextureImage) {
	normalTextureImage = newNormalTextureImage;
}

VkDeviceMemory* Object::getNormalTextureImageMemory() {
	return &normalTextureImageMemory;
}

void Object::setNormalTextureImageMemory(VkDeviceMemory newNormalTextureImageMemory) {
	normalTextureImageMemory = newNormalTextureImageMemory;
}

VkImageView* Object::getNormalTextureImageView() {
	return &normalTextureImageView;
}

void Object::setNormalTextureImageView(VkImageView newNormalTextureImageView) {
	normalTextureImageView = newNormalTextureImageView;
}

VkSampler* Object::getNormalTextureSampler() {
	return &normalTextureSampler;
}

void Object::setNormalTextureSampler(VkSampler newNormalTextureSampler) {
	normalTextureSampler = newNormalTextureSampler;
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