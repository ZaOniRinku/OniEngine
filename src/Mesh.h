#pragma once
#include "../external/glm/glm.hpp"
#include "../external/glm/gtx/hash.hpp"
#include <vulkan/vulkan.hpp>
#include <string>
#define GLM_ENABLE_EXPERIMENTAL

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoords;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 6> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 6> attributeDescriptions = {};
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
		attributeDescriptions[2].offset = offsetof(Vertex, texCoords);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(Vertex, normal);

		attributeDescriptions[4].binding = 0;
		attributeDescriptions[4].location = 4;
		attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[4].offset = offsetof(Vertex, tangent);

		attributeDescriptions[5].binding = 0;
		attributeDescriptions[5].location = 5;
		attributeDescriptions[5].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[5].offset = offsetof(Vertex, bitangent);

		return attributeDescriptions;
	}

	static std::array<VkVertexInputAttributeDescription, 1> getSkyboxAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		return attributeDescriptions;
	}

	static std::array<VkVertexInputAttributeDescription, 1> getShadowsAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		return attributeDescriptions;
	}

	bool operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoords == other.texCoords && normal == other.normal;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoords) << 1) ^
				(hash<glm::vec3>()(vertex.normal) << 1);
		}
	};
}

class Mesh {
public:
	Mesh();
	Mesh(std::string mPath);

	std::string getModelPath();
	void addParametricMeshVertex(double x, double y, double z);
	std::vector<double>& getParametricMeshVertices();
	void addParametricMeshIndex(uint32_t index);
	std::vector<uint32_t>& getParametricMeshIndices();

	int getVertexOffset();
	void setVertexOffset(int newVertexOffset);
	int getIndexOffset();
	void setIndexOffset(int newIndexOffset);
	int getIndexSize();
	void setIndexSize(int newIndexSize);

	bool isConstructed();
	void constructedTrue();
private:
	std::string modelPath;
	std::vector<double> parametricMeshVertices;
	std::vector<uint32_t> parametricMeshIndices;

	int vertexOffset;
	int indexOffset;
	int indexSize;

	bool constructed;
};