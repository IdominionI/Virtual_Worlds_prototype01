#pragma once

#include "geometry_basis.h"

#include <Universal/3rdparty/include/assimp/Importer.hpp>
#include <Universal/3rdparty/include/assimp/postprocess.h>
#include <Universal/3rdparty/include/assimp/scene.h>

class mesh_class : public geometry_basis_class{
  public:
    mesh_class() = default;

    virtual ~mesh_class(){
        delete_buffers();
    }

    //bool load(const std::string& filepath) {
    bool load(const std::string& filepath, float x_off) {
        const uint32_t cMeshImportFlags =
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_SortByPType |
            aiProcess_GenNormals |
            aiProcess_GenUVCoords |
            aiProcess_OptimizeMeshes |
            aiProcess_ValidateDataStructure;

        Assimp::Importer Importer;

        const aiScene* pScene = Importer.ReadFile(filepath.c_str(),
            cMeshImportFlags);

        if (pScene && pScene->HasMeshes())
        {
            vertex_indices.clear();
            vertices.clear();

            auto* mesh = pScene->mMeshes[0];

            for (uint32_t i = 0; i < mesh->mNumVertices; i++)
            {
                point_vertex_class vh;
                vh.mPos = { mesh->mVertices[i].x +x_off, mesh->mVertices[i].y ,mesh->mVertices[i].z };
                vh.mNormal = { mesh->mNormals[i].x, mesh->mNormals[i].y ,mesh->mNormals[i].z };

                add_vertex(vh);
            }

            for (size_t i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];

                for (size_t j = 0; j < face.mNumIndices; j++)
                    add_vertex_index(face.mIndices[j]);
            }

            init();
            number_vertices = vertices.size();
            return true;
        }
        return false;
    }

    // add_vertex function required but cannot be defined as a virtual function as the function paramter
    // needs to be spcified and thus cannot define this function with a different paramater data type
    // Using a template just unneccessarily complicates things as well and causes problems
    void add_vertex(const point_vertex_class& vertex) { vertices.push_back(vertex); }

    void add_vertex_index(unsigned int vertex_idx) { vertex_indices.push_back(vertex_idx); }

    std::vector<unsigned int> get_vertex_indices() { return vertex_indices; }

   bool create_buffers() override {
        vertex_buffer->create_surface_buffers(vertices, vertex_indices);
        return true;
    }

   bool delete_buffers() override {
        vertex_buffer->delete_surface_buffers();
        number_vertices = 0;
    	return true;
   }

    // Vertices and indices
    std::vector<point_vertex_class> vertices;
  };

