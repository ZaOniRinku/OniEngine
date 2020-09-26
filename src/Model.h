#pragma once
#include <string>
#include "Mesh.h"

class Model {
public:
	Model();
	Model(std::string mPath);

	std::string getModelPath();

	void addParametricMeshVertex(double x, double y, double z);
	std::vector<double>& getParametricMeshVertices();
	void addParametricMeshIndex(uint32_t index);
	std::vector<uint32_t>& getParametricMeshIndices();

	std::vector<Mesh>& getMeshes();
	void addMesh(uint64_t indexOffset, uint64_t indexSize);

	uint64_t getVertexOffset();
	void setVertexOffset(uint64_t newVertexOffset);

	bool isConstructed();
	void constructedTrue();
private:
	std::string modelPath;

	std::vector<double> parametricMeshVertices;
	std::vector<uint32_t> parametricMeshIndices;

	std::vector<Mesh> meshes;

	uint64_t vertexOffset;

	bool constructed;
};

