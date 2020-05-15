#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include "SGNode.h"
#define GLM_ENABLE_EXPERIMENTAL

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	glm::vec3 normal;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(Vertex, normal);

		return attributeDescriptions;
	}

	bool operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1) ^
				(hash<glm::vec3>()(vertex.normal) << 1);
		}
	};
}

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
	alignas(16) glm::vec3 camPos;
};

class SGNode;

class Object {
public:
	Object();
	Object(std::string mPath, std::string dtPath, glm::vec3 oPos, float mScale);
	Object(std::string mPath, std::string dtPath, float x, float y, float z, float mScale);
	Object(std::string mPath, std::string dtPath, float x, float y, float z, float mScale, float xRot, float yRot, float zRot);
	void move(glm::vec3 movePosition);
	void move(float x, float y, float z);
	void rescale(float newScale);
	float getPositionX();
	float getPositionY();
	float getPositionZ();
	void setPosition(float newX, float newY, float newZ);
	float getScale();
	float getRotationX();
	float getRotationY();
	float getRotationZ();
	void setRotation(float newRotX, float newRotY, float newRotZ);
	SGNode* getNode();
	void setNode(SGNode* newNode);
	std::string getModelPath();
	std::string getDiffuseTexturePath();
	void setDiffuseTexturePath(std::string newDiffuseTexturePath);
	std::string getNormalTexturePath();
	void setNormalTexturePath(std::string newNormalTexturePath);
	std::string getName();
	void setName(std::string newName);
	std::vector<Vertex>* getModelVertices();
	std::vector<uint32_t>* getModelIndices();
	VkBuffer* getVertexBuffer();
	VkDeviceMemory* getVertexBufferMemory();
	VkBuffer* getIndexBuffer();
	VkDeviceMemory* getIndexBufferMemory();
	uint32_t getDiffuseMipLevel();
	void setDiffuseMipLevel(uint32_t newDiffuseMipLevel);
	uint32_t getNormalMipLevel();
	void setNormalMipLevel(uint32_t newNormalMipLevel);
	VkImage* getDiffuseTextureImage();
	void setDiffuseTextureImage(VkImage newDiffuseTextureImage);
	VkDeviceMemory* getDiffuseTextureImageMemory();
	void setDiffuseTextureImageMemory(VkDeviceMemory newDiffuseTextureImageMemory);
	VkImageView* getDiffuseTextureImageView();
	void setDiffuseTextureImageView(VkImageView newDiffuseTextureImageView);
	VkSampler* getDiffuseTextureSampler();
	void setDiffuseTextureSampler(VkSampler newDiffuseTextureSampler);
	VkImage* getNormalTextureImage();
	void setNormalTextureImage(VkImage newNormalTextureImage);
	VkDeviceMemory* getNormalTextureImageMemory();
	void setNormalTextureImageMemory(VkDeviceMemory newNormalTextureImageMemory);
	VkImageView* getNormalTextureImageView();
	void setNormalTextureImageView(VkImageView newNormalTextureImageView);
	VkSampler* getNormalTextureSampler();
	void setNormalTextureSampler(VkSampler newNormalTextureSampler);
	VkDescriptorSet* getDescriptorSet(int index);
	void addDescriptorSet(VkDescriptorSet* newDescriptorSet);
	VkBuffer* getUniformBuffer(int index);
	void addUniformBuffer(VkBuffer* newUniformBuffer);
	VkDeviceMemory* getUniformBufferMemory(int index);
	void addUniformBufferMemory(VkDeviceMemory* newUniformBufferMemory);
private:
	// Object's attributes
	std::string modelPath;
	std::string diffuseTexturePath;
	std::string normalTexturePath;
	glm::vec3 pos;
	float scale;
	glm::vec3 rot;

	SGNode* node;

	std::string name = "";

	// Model attributes
	std::vector<Vertex> modelVertices;
	std::vector<uint32_t> modelIndices;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	// Textures attributes
	VkImage diffuseTextureImage;
	VkDeviceMemory diffuseTextureImageMemory;
	VkImageView diffuseTextureImageView;
	VkSampler diffuseTextureSampler;
	uint32_t diffuseMipLevel;

	VkImage normalTextureImage;
	VkDeviceMemory normalTextureImageMemory;
	VkImageView normalTextureImageView;
	VkSampler normalTextureSampler;
	uint32_t normalMipLevel;

	std::vector<VkDescriptorSet*> descriptorSets;

	std::vector<VkBuffer*> uniformBuffers;
	std::vector<VkDeviceMemory*> uniformBuffersMemory;
};
