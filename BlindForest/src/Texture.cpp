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

void Texture::CompileTexture()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	textureData = stbi_load(texturePath, &width, &height, &nrChannels, 0);
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

void Texture::UseShader()
{
}

Texture::~Texture()
{
}
