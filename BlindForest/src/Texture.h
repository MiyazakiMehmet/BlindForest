#pragma once

#include <GL/glew.h>

class Texture {
private:
	unsigned int textureID;
public:
	Texture();

	void CompileTexture();
	void UseShader();

	~Texture();
};