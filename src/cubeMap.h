#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <texture2d.h>

class Shader;

class CubeMap
{
	friend class Shader;

	GLuint id;

public:
	CubeMap();
	~CubeMap();
	CubeMap(const CubeMap&) = delete;
	CubeMap& operator=(const CubeMap&) = delete;

	CubeMap(CubeMap&& other) : id(other.id)
	{
		other.id = 0; //Use the "null" texture for the old object.
	}

	CubeMap& operator=(CubeMap&& other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			this->~CubeMap();
			//obj_ is now 0.
			std::swap(id, other.id);
		}
	}

	void bind();
	void load(std::vector<std::string> faces);
	void create(GLint internalFormat, int width, int height, GLenum format, GLenum type);
	void setWrap(Texture2D::Wrap sWrap, Texture2D::Wrap tWrap, Texture2D::Wrap rWrap);
	void setBorderColor(float* rgba);
	void setFiltering(Texture2D::Filtering minFilter, Texture2D::Filtering magFilter);
	void generateMipMaps();
	void setMipMapLevel(int min, int max);
};