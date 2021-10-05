#include "shader.h"
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(std::string vertPath, std::string fragPath)
{
	this->programId = -1;
	std::ifstream vertStr = std::ifstream(vertPath);
	std::string line;
	while (std::getline(vertStr, line))
	{
		this->vert += line + "\n";
	}
	vertStr.close();

	std::ifstream fragStr = std::ifstream(fragPath);
	while (std::getline(fragStr, line))
	{
		this->frag += line + "\n";
	}
	fragStr.close();
}

Shader::~Shader()
{
	if (programId != -1)
	{
		glDeleteProgram(programId);
	}
}

#pragma warning( push )
#pragma warning( disable : 6387 )
void Shader::compile()
{
	char* vertSource = (char*)malloc(this->vert.size() + 1);
	memcpy(vertSource, this->vert.data(), this->vert.size());
	vertSource[this->vert.size()] = '\0';

	GLuint vertId = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertId, 1, &vertSource, NULL);
	glCompileShader(vertId);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertId, 512, NULL, infoLog);
		std::string errMsg = std::string("Vertex Shader Compilation Failed\n") + infoLog;
		throw std::exception(errMsg.c_str());
	}

	free(vertSource);

	char* fragSource = (char*)malloc(this->frag.size() + 1);
	memcpy(fragSource, this->frag.data(), this->frag.size());
	fragSource[this->frag.size()] = '\0';

	GLuint fragId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragId, 1, &fragSource, NULL);
	glCompileShader(fragId);

	glGetShaderiv(fragId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragId, 512, NULL, infoLog);
		std::string errMsg = std::string("Fragment Shader Compilation Failed\n") + infoLog;
		throw std::exception(errMsg.c_str());
	}

	free(fragSource);

	programId = glCreateProgram();
	glAttachShader(programId, vertId);
	glAttachShader(programId, fragId);
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programId, 512, NULL, infoLog);
		std::string errMsg = std::string("Shader Program Compilation Failed\n") + infoLog;
		throw std::exception(errMsg.c_str());
	}

	glDeleteShader(vertId);
	glDeleteShader(fragId);
}
#pragma warning( pop )

void Shader::activate()
{
	if (programId != -1)
	{
		glUseProgram(programId);
	} 
	else
	{
		throw std::exception("Shader program not compiled!");
	}
}

GLint Shader::getUniformLocation(const char* uniform)
{
	GLint val = glGetUniformLocation(programId, uniform);
	if (val == -1)
		throw std::exception("Shader uniform location not found");
	return val;
}

template<>
void Shader::setUniform<int>(const char* name, int value)
{
	glUniform1i(this->getUniformLocation(name), value);
}

template<>
void Shader::setUniform<float>(const char* name, float value)
{
	glUniform1f(this->getUniformLocation(name), value);
}

template<>
void Shader::setUniform<double>(const char* name, double value)
{
	glUniform1d(this->getUniformLocation(name), value);
}

template<>
void Shader::setUniform<bool>(const char* name, bool value)
{
	glUniform1i(this->getUniformLocation(name), (int)value);
}

template<>
void Shader::setUniform<glm::mat4>(const char* name, glm::mat4 value)
{
	glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void Shader::setUniform<glm::vec3>(const char* name, glm::vec3 value)
{
	glUniform3f(this->getUniformLocation(name), value.x, value.y, value.z);
}

template<>
void Shader::setUniform<glm::vec2>(const char* name, glm::vec2 value)
{
	glUniform2f(this->getUniformLocation(name), value.x, value.y);
}

void Shader::activateTexture(Texture2D& texture, const char* uniformName, int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	texture.bind();
	this->setUniform(uniformName, index);
}

void Shader::activateCubemap(CubeMap& texture, const char* uniformName, int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	texture.bind();
	this->setUniform(uniformName, index);
}

template<typename T>
void Shader::setUniform(std::string&& name, T value)
{
	this->setUniform<T>(name.c_str(), value);
}

template void Shader::setUniform<int>(std::string&& name, int value);
template void Shader::setUniform<float>(std::string&& name, float value);
template void Shader::setUniform<double>(std::string&& name, double value);
template void Shader::setUniform<bool>(std::string&& name, bool value);
template void Shader::setUniform<glm::vec2>(std::string&& name, glm::vec2 value);
template void Shader::setUniform<glm::vec3>(std::string&& name, glm::vec3 value);
template void Shader::setUniform<glm::mat4>(std::string&& name, glm::mat4 value);