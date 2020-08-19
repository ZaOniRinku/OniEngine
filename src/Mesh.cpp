#include "Mesh.h"

Mesh::Mesh(std::string mPath) {
	modelPath = mPath;
	constructed = false;
}

std::string Mesh::getModelPath() {
	return modelPath;
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