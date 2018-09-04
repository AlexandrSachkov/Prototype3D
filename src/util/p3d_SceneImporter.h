#pragma once

#include "../p3d_ResourceModel.h"

#include "assimp/matrix4x4.h"
#include "assimp/material.h"

#include <string>
#include <vector>

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiLight;

namespace p3d {
	namespace model {
		struct Scene;
		struct Material;
	}
	namespace util {
		class SceneImporter {
		public:
			SceneImporter();
			~SceneImporter();

			bool import(std::string path, model::Scene& scene);

		private:
			bool loadModels(aiNode* node, aiMatrix4x4 transform, aiMesh** meshes, std::vector<model::Model>& out);
			bool loadMeshes(aiMesh** meshes, unsigned int numMeshes, std::vector<model::Mesh>& out);
			bool loadVertices(aiMesh* mesh, model::Mesh& out);
			bool loadIndices(aiMesh* mesh, model::Mesh& out);
			bool loadMaterials(aiMaterial** materials, unsigned int numMaterials, std::string scenePath, std::vector<model::Material>& out);
			bool loadMaterial(aiMaterial* material, std::string scenePath, model::Material& out);
			bool loadTexture2D(aiMaterial* material, aiTextureType type, std::string scenePath, bool& enabled, model::Texture2D& out);
			bool loadTextureMapMode(aiTextureMapMode in, TEXTURE_MAP_MODE& out);
			bool loadLights(aiLight** lights, unsigned int numLights, std::vector<model::Light>& out);
			bool loadLight(aiLight* in, model::Light& out);
		};
	}
}
