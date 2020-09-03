#include "MemoryAllocator.h"

Chunk::Chunk(VkDevice* device, int32_t memoryType) {
    type = memoryType;
    chunkSize = 268435456;
    chunkSpace = chunkSize;

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    // Default 256 MB size
    allocInfo.allocationSize = 268435456;
    allocInfo.memoryTypeIndex = memoryType;
    if (vkAllocateMemory(*device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate memory (chunk creation)!");
    }

    Block block;
    block.offset = 0;
    block.size = chunkSize;
    block.inUse = false;
    block.next = nullptr;

    initialBlock = block;
}

VkDeviceSize Chunk::allocate(VkMemoryRequirements memRequirements) {
    Block* curr = &initialBlock;
    while (curr != nullptr) {
        if (!curr->inUse) {
            VkDeviceSize actualSize = curr->size;

            // Size of the block minus the alignment (wasted bytes)
            if (curr->offset % memRequirements.alignment != 0) {
                actualSize -= memRequirements.alignment - curr->offset % memRequirements.alignment;
            }

            // If we have enough space to allocate here
            if (actualSize >= memRequirements.size) {
                // Set the size of the block to be the size with alignment
                curr->size = actualSize;

                // Align offset
                if (curr->offset % memRequirements.alignment != 0) {
                    curr->offset += memRequirements.alignment - curr->offset % memRequirements.alignment;
                }

                // If we have exactly enough space, no need to subdivide
                if (curr->size == memRequirements.size) {
                    curr->inUse = true;
                    chunkSpace -= memRequirements.size;
                    return curr->offset;
                }

                // Subdivide the block by creating a new one
                Block newBlock;
                newBlock.inUse = false;
                newBlock.offset = curr->offset + memRequirements.size;
                newBlock.size = curr->size - memRequirements.size;
                newBlock.next = curr->next;

                // The size of the current block is now the size of the data
                curr->size = memRequirements.size;
                curr->inUse = true;
                curr->next = &newBlock;

                chunkSpace -= memRequirements.size;

                return curr->offset;
            }
        }
    }

    return -1;
}

MemoryAllocator::MemoryAllocator() { }

void MemoryAllocator::setDevice(VkDevice* newDevice) {
    device = newDevice;
}

void MemoryAllocator::setPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties newPhysicalDeviceMemoryProperties) {
    memoryProperties = newPhysicalDeviceMemoryProperties;
}

VkDeviceSize MemoryAllocator::allocate(VkBuffer* bufferToAllocate, VkMemoryPropertyFlags flags) {
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(*device, *bufferToAllocate, &memRequirements);
    int32_t properties = findProperties(memRequirements.memoryTypeBits, flags);
	bool found = false;

    // Look for the first block with enough space
	for (Chunk chunk : chunks) {
        if (chunk.type == properties && chunk.chunkSpace >= memRequirements.size) {
            VkDeviceSize offset;
            offset = chunk.allocate(memRequirements);

            if (offset == -1) {
                throw std::runtime_error("failed to allocate memory (block allocation)!");
            }

            vkBindBufferMemory(*device, *bufferToAllocate, chunk.memory, offset);
            return chunk.chunkSpace;
        }
	}

    // No block has been found, create a new chunk
    Chunk newChunk = Chunk(device, properties);

    // Add to this chunk
    newChunk.allocate(memRequirements);
    chunks.push_back(newChunk);

    return newChunk.chunkSpace;
}

VkDeviceSize MemoryAllocator::allocate(VkImage* imageToAllocate, VkMemoryPropertyFlags flags) {
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(*device, *imageToAllocate, &memRequirements);
    int32_t properties = findProperties(memRequirements.memoryTypeBits, flags);
    bool found = false;

    // Look for the first block with enough space
    for (Chunk chunk : chunks) {
        if (chunk.type == properties && chunk.chunkSpace >= memRequirements.size) {
            VkDeviceSize offset;
            offset = chunk.allocate(memRequirements);
            vkBindImageMemory(*device, *imageToAllocate, chunk.memory, offset);
            return chunk.chunkSpace;
        }
    }

    // No block has been found, create a new chunk
    Chunk newChunk = Chunk(device, properties);

    // Add to this chunk
    newChunk.allocate(memRequirements);
    chunks.push_back(newChunk);

    return newChunk.chunkSpace;
}

void MemoryAllocator::free() {
    for (Chunk chunk : chunks) {
        vkFreeMemory(*device, chunk.memory, nullptr);
    }
}

int32_t MemoryAllocator::findProperties(uint32_t memoryTypeBitsRequirement, VkMemoryPropertyFlags requiredProperties) {
    const uint32_t memoryCount = memoryProperties.memoryTypeCount;
    for (uint32_t memoryIndex = 0; memoryIndex < memoryCount; memoryIndex++) {
        const uint32_t memoryTypeBits = (1 << memoryIndex);
        const bool isRequiredMemoryType = memoryTypeBitsRequirement & memoryTypeBits;

        const VkMemoryPropertyFlags properties = memoryProperties.memoryTypes[memoryIndex].propertyFlags;
        const bool hasRequiredProperties = (properties & requiredProperties) == requiredProperties;

        if (isRequiredMemoryType && hasRequiredProperties) {
            return static_cast<int32_t>(memoryIndex);
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}