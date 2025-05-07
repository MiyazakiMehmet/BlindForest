#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

//Pointer points the address of array instead of just first element
void Mesh::CompileMesh(float* vertices, unsigned int* indices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO); //Generates an id for VAO
	glBindVertexArray(VAO); //From now on all implemantaion and functions stored in VAO

	glGenBuffers(1, &VBO); //Generates an id for VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //From now on only array buffer implemantaion and functions stored in VBO

	glGenBuffers(1, &IBO); //Generates an id for IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); //From now on only element array buffer implemantaion and functions stored in VBO

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); //Unbind

}

void Mesh::CompileMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	indexCount = (unsigned int)indices.size();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),
		vertices.data(),
		GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		indices.data(),
		GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO); //This VAO holds all VBO and IBO datas
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}
