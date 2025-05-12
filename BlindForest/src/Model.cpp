#include "Model.h"

Model::Model()
{
}

void Model::LoadModel(std::string& filePath)
{
}

void Model::RenderModel()
{
}

void Model::ClearModel()
{
}


void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	//Loads Meshes within that node (e.g. Arm Mesh, Head Mesh)
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	//Loads nodes within that node (e.g. Arm node->finger node -> finger mesh)
	for (size_t i = 0; i < node->mNumChildren; i++) {
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	//Vertices Load
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		//Positions
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		//Textures
		if (mesh->mTextureCoords[0]) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		//Normals
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,  -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	//Indices Load
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CompileMesh(&vertices[0], &indices[0], indices.size());
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterial(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);
	std::cout << "Model has " << (int)scene->mNumMaterials << "materials" << std::endl;
	for (size_t i = 0; i < textureList.size(); i++) {
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {

				std::cout << "Texture found: " << path.C_Str() << std::endl;
				int idx = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("Textures/") + fileName;

				//textureList[i] = new Texture(texPath.c_str());
				//
				//if (!textureList[i]->LoadTexture()) {
				//	printf("Failed to load texture at: %s", texPath);
				//
				//	delete textureList[i];
				//	textureList[i] = nullptr;
				//}
			}
		}
	}
}

Model::~Model()
{
}
