#pragma once

#include <GL/glew.h>

#include <vector>
#include <string>

#include "Mesh.h"
#include "Texture.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model {
private:
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterial(const aiScene* scene);

	std::vector<Mesh* > meshList;
	std::vector<Texture* > textureList;
	std::vector<unsigned int> meshToTex;
public:
	Model();

	void LoadModel(std::string& filePath);
	void RenderModel();
	void ClearModel();

	~Model();
};