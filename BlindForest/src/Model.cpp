#include "Model.h"

Model::Model()
{
}

void Model::LoadModel(std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene) {
		std::cout << "Model " << filePath << " failed to load: " << importer.GetErrorString() << std::endl;

		return;
	}

	//this will get the very first node in the scene (the one thats on the center)
	LoadNode(scene->mRootNode, scene);

	//load Texture 
	LoadMaterial(scene);
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++) {

		unsigned int materialIndex = meshToTex[i]; //grabbing material index

		//check whether index valids
		if (materialIndex < textureList.size() && textureList[materialIndex]) {
			textureList[materialIndex]->UseTexture();
		}
		//first drawed texture and draw mesh
		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++) {
		if (meshList[i]) {
			delete meshList[i];
			meshList[i] = nullptr;
		}

		if (textureList[i]) {
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
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
		vertices.insert(vertices.end(), { mesh->mNormals[i].x,  mesh->mNormals[i].y, mesh->mNormals[i].z });
	}

	//Indices Load
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CompileMesh(vertices, indices);
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

				std::string texPath = std::string("src/textures/") + fileName;

				textureList[i] = new Texture(texPath.c_str());
				
				if (!textureList[i]->LoadTexture()) {
						std::cout << "Failed to load texture at: " << texPath << std::endl;

					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}
		//If model does not have a diffuse texture assigned to assimp texture will not be found.
		if (textureList[i] == nullptr) {
			std::cout << "No DIFFUSE textures found for material: " << i << " assigning fallback." << std::endl;

			// Set missing texture with existing one
			std::string fallbackTex = "src/textures/bark_0004.jpg";
			textureList[i] = new Texture(fallbackTex.c_str());

			if (!textureList[i]->LoadTexture()) {
				std::cout << "Failed to load fallback texture for material " << i << std::endl;
				delete textureList[i];
				textureList[i] = nullptr;
			}
			else {
				std::cout << "Fallback texture assigned to material " << i << std::endl;
			}
		}
	}
}

Model::~Model()
{
	ClearModel();
}
