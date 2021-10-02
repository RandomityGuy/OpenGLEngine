#pragma once
#include <string>
#include "texture2d.h"
#include "cubeMap.h"
#include <glad/glad.h>

class Shader
{
	std::string vert;
	std::string frag;

	GLuint programId;
public:
	Shader(std::string vertPath, std::string fragPath);
	~Shader();
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	Shader(Shader&& other) : programId(other.programId)
	{
		other.programId = 0; //Use the "null" texture for the old object.
	}

	Shader& operator=(Shader&& other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			this->~Shader();
			//obj_ is now 0.
			std::swap(programId, other.programId);
		}
	}

	void compile();
	void activate();

	GLint getUniformLocation(const char* uniform);
	// utility uniform functions
	template<typename T>
	void setUniform(const char* name, T value);

	template<typename T>
	void setUniform(std::string&& name, T value);

	void activateTexture(Texture2D& texture, const char* uniformName, int index);
	void activateCubemap(CubeMap& texture, const char* uniformName, int index);

};