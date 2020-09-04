#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include <algorithm>

#define CHUNK_SIZE 268435456

struct Block {
	Block* next;

	VkDeviceSize offset;
	VkDeviceSize size;
	bool inUse;
};

struct Chunk {
	VkDeviceMemory memory;
	int32_t type;
	Block* head;

	Chunk(VkDevice* device, int32_t memoryType, VkDeviceSize size);
	VkDeviceSize allocate(VkMemoryRequirements memRequirements);
	void freeBlocks();
};

class MemoryAllocator {
public:
	void setDevice(VkDevice* newDevice);
	void setPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties newPhysicalDeviceMemoryProperties);
	VkDeviceSize allocate(VkBuffer* bufferToAllocate, VkMemoryPropertyFlags flags);
	VkDeviceSize allocate(VkImage* imageToAllocate, VkMemoryPropertyFlags flags);
	void free();
	int32_t findProperties(uint32_t memoryTypeBitsRequirement, VkMemoryPropertyFlags requiredProperties);
private:
	VkDevice* device;
	VkPhysicalDeviceMemoryProperties memoryProperties;
	std::vector<Chunk> chunks;
};