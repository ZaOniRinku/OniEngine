#include "Mesh.h"

Mesh::Mesh(std::string mPath) {
	modelPath = mPath;
	constructed = false;
}

std::string Mesh::getModelPath() {
	return modelPath;
}

std::vector<Vertex>* Mesh::getVertices() {
	return &vertices;
}

std::vector<uint32_t>* Mesh::getIndices() {
	return &indices;
}

VkBuffer* Mesh::getVertexBuffer() {
	return &vertexBuffer;
}

VkDeviceMemory* Mesh::getVertexBufferMemory() {
	return &vertexBufferMemory;
}

VkBuffer* Mesh::getIndexBuffer() {
	return &indexBuffer;
}

VkDeviceMemory* Mesh::getIndexBufferMemory() {
	return &indexBufferMemory;
}

bool Mesh::isConstructed() {
	return constructed;
}

void Mesh::constructedTrue() {
	constructed = true;
}