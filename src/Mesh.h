#pragma once
#include <cstdint>

class Mesh {
public:
	Mesh(uint64_t indexO, uint64_t indexS);

	uint64_t getIndexOffset();
	void setIndexOffset(uint64_t newIndexOffset);
	uint64_t getIndexSize();
	void setIndexSize(uint64_t newIndexSize);
private:
	uint64_t indexOffset;
	uint64_t indexSize;
};