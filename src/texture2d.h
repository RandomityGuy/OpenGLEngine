#pragma once
#include <glad/glad.h>
#include <vector>

class Shader;
class FrameBuffer;

class Texture2D
{
	friend class Shader;
	friend class FrameBuffer;
	GLuint id;

public:
	Texture2D();
	~Texture2D();
	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	Texture2D(Texture2D&& other) : id(other.id)
	{
		other.id = 0; //Use the "null" texture for the old object.
	}

	Texture2D& operator=(Texture2D&& other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			this->~Texture2D();
			//obj_ is now 0.
			std::swap(id, other.id);
		}
	}

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
	void create(GLint internalFormat, int width, int height, GLenum format, GLenum type);
	void bind();
	void setWrap(Wrap sWrap, Wrap tWrap);
	void setBorderColor(float* rgba);
	void setFiltering(Filtering minFilter, Filtering magFilter);
	void generateMipMaps();
	void setMipMapLevel(int min, int max);
};