#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <texture2d.h>
#include <vertexBuffer.h>
#include <indexBuffer.h>
#include <vertexArray.h>

struct RenderContext;

class Mesh
{
	bool _loadedMaterialTextures = false;
	bool _generatedBuffers = false;
public:

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	struct VertexGroup
	{
		std::vector<Vertex> vertices;
		std::vector<int> indices;
		int materialIndex;

		VertexArray* VAO = NULL;
		VertexBuffer* VBO = NULL;
		IndexBuffer* EBO = NULL;
	};

	struct Material
	{
		std::string name;
		glm::vec3 ambientColor;
		std::string ambientTex;
		Texture2D* ambientTexture2D = NULL;
		glm::vec3 diffuseColor;
		std::string diffuseTex;
		Texture2D* diffuseTexture2D = NULL;
		glm::vec3 specularColor;
		std::string specularTex;
		Texture2D* specularTexture2D = NULL;
		glm::vec3 emissionColor;
		std::string emissionTex;
		Texture2D* emissionTexture2D = NULL;
		float shininess;
		float reflectivity;
		float alpha = 1;
		bool transparent = false;
	};

	std::vector<VertexGroup> vertexGroups;
	std::vector<Material> materials;

	Mesh();
	~Mesh();

	void load(const char* path);
	void loadMaterialTextures();
	void disposeMaterialTextures();
	virtual void generateBuffers();
	virtual void disposeBuffers();
	virtual void render(RenderContext* context);
};