#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	texturePath = "";
	width = 0;
	height = 0;
	nrChannels = 0;
}

Texture::Texture(const char* texPath) {
	textureID = 0;
	texturePath = texPath;
	width = 0;
	height = 0;
	nrChannels = 0;
}

// Loads texture from file (returning bool for model loading instead of void) 
bool Texture::LoadTexture() {
	stbi_set_flip_vertically_on_load(true);
	textureData = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	if (!textureData) {
		std::cout << "Failed to find: " << texturePath << std::endl;

		return false;
	}

	CompileTexture();

	return true;
}

void Texture::CompileTexture()
{
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (!textureData) {
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(textureData);
	}

	GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);

	std::cout << "Loaded texture: " << texturePath << " with " << nrChannels << " channels." << std::endl;

	stbi_image_free(textureData);
	glBindTexture(GL_TEXTURE_2D, 0); //Unbind
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	nrChannels = 0;
	texturePath = "";
}
