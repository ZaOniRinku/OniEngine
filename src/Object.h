#pragma once
#include "../external/glfw/include/GLFW/glfw3.h"
#include "SGNode.h"
#include "Material.h"
#include "Mesh.h"

struct ObjectBufferObject {
	alignas(16) glm::mat4 model;
};

class SGNode;

class Object {
public:
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
	std::vector<VkDescriptorSet>* getDescriptorSets();
	std::vector<VkDescriptorSet>* getShadowsDescriptorSets();
	std::vector<VkBuffer>* getObjectBuffers();
	std::vector<VkDeviceMemory>* getObjectBufferMemories();
	int getGraphicsPipelineIndex();
	void setGraphicsPipelineIndex(int newGraphicsPipelineIndex);

	void (*frameEvent)(Object *obj, GLFWwindow* window, double deltaTime);
private:
	Mesh *mesh;
	Material *material;

	std::string name = "";

	float posX;
	float posY;
	float posZ;

	float rotX;
	float rotY;
	float rotZ;

	float scale;

	std::vector<VkBuffer> objectBuffers;
	std::vector<VkDeviceMemory> objectBufferMemories;

	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkDescriptorSet> shadowsDescriptorSets;

	SGNode* node;

	int graphicsPipelineIndex;
};