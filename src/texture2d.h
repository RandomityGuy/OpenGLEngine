#pragma once
#include <glad/glad.h>

class Shader;

class Texture2D
{
	friend class Shader;
	GLuint id;

public:
	Texture2D();
	~Texture2D();

	int width;
	int height;
	int numChannels;
	
	enum Wrap
	{
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER
	};

	enum Filtering
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMimMapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
	};

	void load(const char* path);
	void bind();
	void setWrap(Wrap sWrap, Wrap tWrap);
	void setBorderColor(float* rgba);
	void setFiltering(Filtering minFilter, Filtering magFilter);
	void generateMipMaps();
};