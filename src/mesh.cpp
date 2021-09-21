#include "mesh.h"
#include <tiny_obj_loader.h>
#include <iostream>
#include <string>
#include <GL.h>
#include "shader.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::load(const char* path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;


	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		throw std::exception("Can't load model!");
	}

    std::map<int, Mesh::VertexGroup> materialFaceMap;

    this->disposeBuffers();
    this->vertexGroups.clear();

    this->disposeMaterialTextures();
    this->materials.clear();

    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;

        std::map<int, int> indexMap;

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // per-face material
            int matId = shapes[s].mesh.material_ids[f];

            Mesh::VertexGroup* face = nullptr;

            if (materialFaceMap.find(matId) != materialFaceMap.end())
            {
                face = &materialFaceMap[matId];
            }
            else
            {
                materialFaceMap.insert(std::make_pair(matId, Mesh::VertexGroup()));
                face = &materialFaceMap[matId];

                tinyobj::material_t& mat = materials[matId];
                Mesh::Material faceMat;
                faceMat.name = mat.name;
                faceMat.ambientColor = glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
                faceMat.ambientTex = mat.ambient_texname;
                faceMat.diffuseColor = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
                faceMat.diffuseTex = mat.diffuse_texname;
                faceMat.specularColor = glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]);
                faceMat.specularTex = mat.specular_texname;
                faceMat.emissionColor = glm::vec3(mat.emission[0], mat.emission[1], mat.emission[2]);
                faceMat.emissionTex = mat.emissive_texname;
                faceMat.shininess = mat.shininess;
                face->materialIndex = this->materials.size();
                this->materials.push_back(faceMat);
            }

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                if (indexMap.find(idx.vertex_index) != indexMap.end())
                {
                    face->indices.push_back(indexMap[idx.vertex_index]);
                    continue;
                }

                Mesh::Vertex vert;

                float vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                float vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                float vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                glm::vec3 pos(vx, vy, vz);
                vert.position = pos;

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    float nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    float ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    float nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    glm::vec3 normal(nx, ny, nz);
                    vert.normal = normal;
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    float tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    float ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    glm::vec2 uv(tx, ty);
                    vert.uv = uv;
                }

                indexMap.insert(std::make_pair(idx.vertex_index, face->vertices.size()));

                face->vertices.push_back(vert);
            }
            index_offset += fv;
        }
    }

    for (auto& vgroup : materialFaceMap)
    {
        this->vertexGroups.push_back(vgroup.second);
    }
}

void Mesh::loadMaterialTextures()
{
    if (_loadedMaterialTextures)
        return;
    for (auto& material : materials)
    {
        if (!material.diffuseTex.empty())
        {
            material.diffuseTexture2D = new Texture2D();
            material.diffuseTexture2D->load(material.diffuseTex.c_str());
        }
        if (!material.ambientTex.empty())
        {
            material.ambientTexture2D = new Texture2D();
            material.ambientTexture2D->load(material.ambientTex.c_str());
        }
        if (!material.specularTex.empty())
        {
            material.specularTexture2D = new Texture2D();
            material.specularTexture2D->load(material.specularTex.c_str());
        }
        if (!material.emissionTex.empty())
        {
            material.emissionTexture2D = new Texture2D();
            material.emissionTexture2D->load(material.emissionTex.c_str());
        }
    }
    _loadedMaterialTextures = true;
}

void Mesh::disposeMaterialTextures()
{
    for (auto& material : materials)
    {
        if (material.ambientTexture2D != NULL) delete material.ambientTexture2D;
        if (material.diffuseTexture2D != NULL) delete material.diffuseTexture2D;
        if (material.specularTexture2D != NULL) delete material.specularTexture2D;
        if (material.emissionTexture2D != NULL) delete material.emissionTexture2D;
        material.ambientTexture2D = NULL;
        material.diffuseTexture2D = NULL;
        material.specularTexture2D = NULL;
        material.emissionTexture2D = NULL;
    }
    _loadedMaterialTextures = false;
}

void Mesh::generateBuffers()
{
    if (this->_generatedBuffers)
        return;
    for (auto& v : vertexGroups)
    {
        if (v.VBO == NULL) v.VBO = new VertexBuffer();
        if (v.EBO == NULL) v.EBO = new IndexBuffer();
        if (v.VAO == NULL) v.VAO = new VertexArray();

        v.VAO->bind();

        v.VBO->uploadData(v.vertices.size() * sizeof(Mesh::Vertex), v.vertices.data(), VertexBuffer::Static);
        v.EBO->uploadData(v.indices.size() * sizeof(int), v.indices.data(), IndexBuffer::Static);

        GL::vertexAttribPointer(0, 3, GL_FLOAT, 2 * 3 * sizeof(float) + 2 * sizeof(float), 0, false);
        GL::enableVertexAttribArray(0);

        GL::vertexAttribPointer(1, 3, GL_FLOAT, 2 * 3 * sizeof(float) + 2 * sizeof(float), 3 * sizeof(float), false);
        GL::enableVertexAttribArray(1);

        GL::vertexAttribPointer(2, 2, GL_FLOAT, 2 * 3 * sizeof(float) + 2 * sizeof(float), 2 * 3 * sizeof(float), false);
        GL::enableVertexAttribArray(2);

        GL::bindBuffer(GL::BufferTarget::ArrayBuffer, 0);
        GL::bindVertexArray(0);
    }
    this->_generatedBuffers = true;
}

void Mesh::disposeBuffers()
{
    for (auto& v : vertexGroups)
    {
        if (v.VAO != NULL) delete v.VAO;
        if (v.VBO != NULL) delete v.VBO;
        if (v.EBO != NULL) delete v.EBO;
        v.VAO = NULL;
        v.VBO = NULL;
        v.EBO = NULL;
    }
    this->_generatedBuffers = false;
}

void Mesh::render(Shader* shader)
{
    this->generateBuffers();
    // Set the camera and the transform uniforms from the model class, not here

    for (auto& vgroup : vertexGroups)
    {
        

        Mesh::Material& material = this->materials[vgroup.materialIndex];
        if (material.ambientTexture2D != NULL)
        {
            shader->activateTexture(*material.ambientTexture2D, "material.ambient", 0);
            shader->setUniform("hasAmbient", true);
        }
        else
            shader->setUniform("hasAmbient", false);

        if (material.diffuseTexture2D != NULL)
        {
            shader->activateTexture(*material.diffuseTexture2D, "material.diffuse", 1);
            shader->setUniform("hasDiffuse", true);
        }
        else
            shader->setUniform("hasDiffuse", false);

        if (material.specularTexture2D != NULL)
        {
            shader->activateTexture(*material.specularTexture2D, "material.specular", 2);
            shader->setUniform("hasSpecular", true);
        }
        else
            shader->setUniform("hasSpecular", false);

        if (material.emissionTexture2D != NULL)
        {
            shader->activateTexture(*material.emissionTexture2D, "material.emission", 3);
            shader->setUniform("hasEmission", true);
        }
        else
            shader->setUniform("hasEmission", false);

        shader->setUniform("material.ambientColor", material.ambientColor);
        shader->setUniform("material.diffuseColor", material.diffuseColor);
        shader->setUniform("material.specularColor", material.specularColor);
        shader->setUniform("material.emissionColor", material.emissionColor);

        shader->setUniform("material.shininess", material.shininess);

        vgroup.VAO->bind();
        glDrawElements(GL_TRIANGLES, vgroup.indices.size(), GL_UNSIGNED_INT, 0);
        GL::bindVertexArray(0);
    }

}
