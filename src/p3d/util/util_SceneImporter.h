#pragma once

#include "../ResourceDescriptions.h"
#include "util_SceneGraphI.h"
#include "../scene/SceneI.h"

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
	namespace util {
		class SceneImporter {
		public:
			static bool import(const std::string& path, SceneI* scene, SceneGraphI* sceneGraph);

		private:
            //static bool loadModels(aiNode* node, aiMatrix4x4 transform, aiMesh** meshes, std::vector<model::Model>& out);
            static bool loadMeshes(aiMesh** meshes, unsigned int numMeshes, std::vector<MeshDesc>& meshesOut);
            /*static bool loadMaterials(aiMaterial** materials, unsigned int numMaterials, std::string scenePath, std::vector<model::Material>& out);
            static bool loadMaterial(aiMaterial* material, std::string scenePath, model::Material& out);
            static bool loadTexture2D(aiMaterial* material, aiTextureType type, std::string scenePath, bool& enabled, model::Texture2D& out);
            static bool loadTextureMapMode(aiTextureMapMode in, TEXTURE_MAP_MODE& out);
            static bool loadLights(aiLight** lights, unsigned int numLights, std::vector<model::Light>& out);
            static bool loadLight(aiLight* in, model::Light& out);*/
		};
	}
}
