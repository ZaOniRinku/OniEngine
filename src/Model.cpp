#include "Model.h"

Model::Model() {
	modelPath = "";
	constructed = false;
}

Model::Model(std::string mPath) {
	modelPath = mPath;
	constructed = false;
}

std::string Model::getModelPath() {
	return modelPath;
}

void Model::addParametricMeshVertex(double x, double y, double z) {
	parametricMeshVertices.push_back(x);
	parametricMeshVertices.push_back(y);
	parametricMeshVertices.push_back(z);
}

std::vector<double>& Model::getParametricMeshVertices() {
	return parametricMeshVertices;
}

void Model::addParametricMeshIndex(uint32_t index) {
	parametricMeshIndices.push_back(index);
}

std::vector<uint32_t>& Model::getParametricMeshIndices() {
	return parametricMeshIndices;
}

std::vector<Mesh>& Model::getMeshes() {
	return meshes;
}

void Model::addMesh(uint64_t indexOffset, uint64_t indexSize) {
	meshes.push_back(Mesh(indexOffset, indexSize));
}

uint64_t Model::getVertexOffset() {
	return vertexOffset;
}

void Model::setVertexOffset(uint64_t newVertexOffset) {
	vertexOffset = newVertexOffset;
}

bool Model::isConstructed() {
	return constructed;
}

void Model::constructedTrue() {
	constructed = true;
}