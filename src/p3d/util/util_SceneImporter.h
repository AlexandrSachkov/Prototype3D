#pragma once

#include "../ResourceDescriptions.h"
#include "util_SceneGraphI.h"
#include "../scene/SceneI.h"

#include "assimp/matrix4x4.h"
#include "assimp/material.h"

#include <string>
#include <vector>
#include <unordered_map>

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiLight;
struct aiTexture;

namespace p3d {
	namespace util {
		class SceneImporter {
		public:
			static bool import(const std::string& path, const std::string& texDir, SceneI* scene, SceneGraphI* sceneGraph);

		private:
            static void loadModels(
                aiNode* srcNode, 
                aiMatrix4x4 transform, 
                aiMesh** srcMeshes, 
                const std::vector<HMesh>& meshes,
                const std::vector<HMaterial>& materials,
                SceneI* scene
            );
            static bool loadMeshes(aiMesh** meshes, unsigned int numMeshes, SceneI* scene, std::vector<HMesh>& meshesOut);
            static bool loadMesh(aiMesh* mesh, MeshDesc& out);
            static bool loadMaterials(
                aiMaterial** materials, 
                unsigned int numMaterials, 
                SceneI* scene,
                const std::string& scenePath,
                const std::string& texDir,
                std::vector<HMaterial>& out
            );
            static bool loadMaterial(
                aiMaterial* material, 
                std::unordered_map<std::string, HTexture2dArr>& texMap, 
                SceneI* scene,
                const std::string& scenePath,
                const std::string& texDir,
                MaterialDesc& out
            );
            static bool loadTexture2D(
                aiMaterial* material, 
                aiTextureType type, 
                std::unordered_map<std::string, HTexture2dArr>& texMap, 
                SceneI* scene,
                const std::string& scenePath,
                const std::string& texDir,
                HTexture2dArr& out, 
                TEX_MAP_MODE& mapMode
            );
            static bool loadTextureMapMode(aiTextureMapMode in, TEX_MAP_MODE& out);
            static bool findTexturePath(
                const std::string& scenePath,
                const std::string& userTexDir,
                const std::string& bakedTexPath,
                std::string& finalPath
            );
            /*static bool loadLights(aiLight** lights, unsigned int numLights, std::vector<model::Light>& out);
            static bool loadLight(aiLight* in, model::Light& out);*/
		};
	}
}
