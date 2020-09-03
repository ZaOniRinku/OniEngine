#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>

struct Block {
	Block* next;

	VkDeviceSize offset;
	VkDeviceSize size;
	bool inUse;
};

struct Chunk {
	VkDeviceMemory memory;
	int32_t type;
	VkDeviceSize chunkSize;
	VkDeviceSize chunkSpace;
	Block initialBlock;

	Chunk(VkDevice* device, int32_t memoryType);
	VkDeviceSize allocate(VkMemoryRequirements memRequirements);
};

class MemoryAllocator {
public:
	MemoryAllocator();
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