#pragma once
#include <string>
#include "texture2d.h"
#include <glad/glad.h>

class Shader
{
	std::string vert;
	std::string frag;

	GLuint programId;
public:
	Shader(std::string vertPath, std::string fragPath);
	~Shader();

	void compile();
	void activate();

	GLint getUniformLocation(const char* uniform);
	// utility uniform functions
	template<typename T>
	void setUniform(const char* name, T value);
	void activateTexture(Texture2D texture, int index);

};