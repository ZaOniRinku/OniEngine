#include "Mesh.h"

Mesh::Mesh(uint64_t indexO, uint64_t indexS) {
	indexOffset = indexO;
	indexSize = indexS;
}

uint64_t Mesh::getIndexOffset() {
	return indexOffset;
}

void Mesh::setIndexOffset(uint64_t newIndexOffset) {
	indexOffset = newIndexOffset;
}

uint64_t Mesh::getIndexSize() {
	return indexSize;
}

void Mesh::setIndexSize(uint64_t newIndexSize) {
	indexSize = newIndexSize;
}