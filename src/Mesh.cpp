#include "Mesh.h"

Mesh::Mesh() {
	modelPath = "";
	constructed = false;
}

Mesh::Mesh(std::string mPath) {
	modelPath = mPath;
	constructed = false;
}

std::string Mesh::getModelPath() {
	return modelPath;
}

void Mesh::addParametricMeshVertex(double x, double y, double z) {
	parametricMeshVertices.push_back(x);
	parametricMeshVertices.push_back(y);
	parametricMeshVertices.push_back(z);
}

std::vector<double>& Mesh::getParametricMeshVertices() {
	return parametricMeshVertices;
}

void Mesh::addParametricMeshIndex(uint32_t index) {
	parametricMeshIndices.push_back(index);
}

std::vector<uint32_t>& Mesh::getParametricMeshIndices() {
	return parametricMeshIndices;
}

int Mesh::getVertexOffset() {
	return vertexOffset;
}

void Mesh::setVertexOffset(int newVertexOffset) {
	vertexOffset = newVertexOffset;
}

int Mesh::getIndexOffset() {
	return indexOffset;
}

void Mesh::setIndexOffset(int newIndexOffset) {
	indexOffset = newIndexOffset;
}

int Mesh::getIndexSize() {
	return indexSize;
}

void Mesh::setIndexSize(int newIndexSize) {
	indexSize = newIndexSize;
}

bool Mesh::isConstructed() {
	return constructed;
}

void Mesh::constructedTrue() {
	constructed = true;
}