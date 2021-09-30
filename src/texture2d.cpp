#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture2d.h"

Texture2D::Texture2D()
{
	glGenTextures(1, &id);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &id);
}

void Texture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::load(const char* path)
{
	unsigned char* data = stbi_load(path, &width, &height, &numChannels, 0);

	GLenum format;
	if (numChannels == 1)
		format = GL_RED;
	else if (numChannels == 3)
		format = GL_RGB;
	else if (numChannels == 4)
		format = GL_RGBA;


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

void Texture2D::setWrap(Wrap sWrap, Wrap tWrap)
{
	this->bind();
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
}

void Texture2D::setBorderColor(float* rgba)
{
	this->bind();
	glTextureParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, rgba);
}

void Texture2D::setFiltering(Filtering minFilter, Filtering magFilter)
{
	this->bind();
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture2D::generateMipMaps()
{
	this->bind();
	glGenerateMipmap(GL_TEXTURE_2D);
}