#pragma once
#include <iostream>
#include <vector>

#include <GL/glew.h>

class Mesh
{
private:
	unsigned int VAO, VBO, IBO;
	unsigned int indexCount;
public:
	Mesh();

	void CompileMesh(float* vertices, unsigned int* indices, unsigned int numOfIndices);
	void CompileMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
	void RenderMesh();

	~Mesh();
};

