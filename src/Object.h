#pragma once
#include <GLFW/glfw3.h>
#include "SGNode.h"
#include "Material.h"
#include "Mesh.h"

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
};

class SGNode;

class Object {
public:
	Object();
	Object(float x, float y, float z, float mScale);
	Object(float x, float y, float z, float mScale, float xRot, float yRot, float zRot);
	void move(float x, float y, float z);
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
	std::string getName();
	void setName(std::string newName);
	Mesh* getMesh();
	void setMesh(Mesh *newMesh);
	Material* getMaterial();
	void setMaterial(Material* newMaterial);
	VkDescriptorSet* getDescriptorSet(int index);
	void addDescriptorSet(VkDescriptorSet* newDescriptorSet);
	VkDescriptorSet* getShadowsDescriptorSet(int index);
	void addShadowsDescriptorSet(VkDescriptorSet* newShadowsDescriptorSet);
	VkBuffer* getUniformBuffer(int index);
	void addUniformBuffer(VkBuffer* newUniformBuffer);
	VkDeviceMemory* getUniformBufferMemory(int index);
	void addUniformBufferMemory(VkDeviceMemory* newUniformBufferMemory);
	VkPipeline* getGraphicsPipeline();
	void setGraphicsPipeline(VkPipeline* newGraphicsPipeline);
	void(*frameEvent)(Object *obj, GLFWwindow* window, double deltaTime);
private:
	// Object attributes

	Mesh *mesh;
	Material *material;

	std::string name = "";
	glm::vec3 pos;
	float scale;
	glm::vec3 rot;

	SGNode* node;

	std::vector<VkDescriptorSet*> descriptorSets;
	std::vector<VkDescriptorSet*> shadowsDescriptorSets;

	std::vector<VkBuffer*> uniformBuffers;
	std::vector<VkDeviceMemory*> uniformBuffersMemory;

	VkPipeline* graphicsPipeline;
};
