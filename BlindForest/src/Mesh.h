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

	void CompileMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices);
	void RenderMesh();

	~Mesh();
};

