#include "cubeMap.h"
#include <stb_image.h>

CubeMap::CubeMap()
{
	glGenTextures(1, &this->id);
}

CubeMap::~CubeMap()
{
	glDeleteTextures(1, &this->id);
}

void CubeMap::bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
}

void CubeMap::load(std::vector<std::string> faces)
{
	this->bind();

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			throw new std::exception("Cubemap tex failed to load");
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::setWrap(Texture2D::Wrap sWrap, Texture2D::Wrap tWrap, Texture2D::Wrap rWrap)
{
	this->bind();
	glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, sWrap);
	glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tWrap);
	glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, rWrap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::setBorderColor(float* rgba)
{
	this->bind();
	glTextureParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, rgba);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::setFiltering(Texture2D::Filtering minFilter, Texture2D::Filtering magFilter)
{
	this->bind();
	glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
	glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::generateMipMaps()
{
	this->bind();
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::setMipMapLevel(int min, int max)
{
	this->bind();
	glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, min);
	glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, max);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}