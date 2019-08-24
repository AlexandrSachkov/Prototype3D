#include "util_SceneImporter.h"
#include "../assert.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/gtc/type_ptr.hpp>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <experimental/filesystem>

namespace p3d {
	namespace util {
		bool SceneImporter::import(const std::string& path, SceneI* sceneOut, SceneGraphI* sceneGraph) {
			std::experimental::filesystem::path scenePath(path);
			if (!std::experimental::filesystem::exists(scenePath))
				return false;
			scenePath = scenePath.remove_filename();

			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path,
				aiProcess_CalcTangentSpace
				| aiProcess_Triangulate
				| aiProcess_JoinIdenticalVertices
				| aiProcess_SortByPType
				| aiProcess_GenSmoothNormals
				| aiProcess_PreTransformVertices
				| aiProcess_ValidateDataStructure
				| aiProcess_RemoveRedundantMaterials
			);

            P3D_ASSERT_R(scene, importer.GetErrorString());
			
            std::vector<MeshDesc> meshes;
			if (!scene->HasMeshes() || !loadMeshes(scene->mMeshes, scene->mNumMeshes, meshes))
				return false;
			/*if (!scene->HasMaterials() || !loadMaterials(scene->mMaterials, scene->mNumMaterials, scenePath.string(), out.materials))
				return false;
			if (scene->HasLights() && !loadLights(scene->mLights, scene->mNumLights, out.lights))
				return false;

            aiMatrix4x4 identity;
            if (!loadModels(scene->mRootNode, identity, scene->mMeshes, out.models))
                return false;*/

			return true;
		}

		/*bool SceneImporter::loadModels(aiNode* node, aiMatrix4x4 parentTransform, aiMesh** meshes, std::vector<model::Model>& out) {
			aiMatrix4x4 transform = parentTransform * node->mTransformation;
			for (unsigned int i = 0; i < node->mNumMeshes; i++) {
				model::Model newModel = {};
				newModel.transform = glm::make_mat4x4((const float*)(&transform));
				unsigned int meshIndex = node->mMeshes[i];
				newModel.meshIndex = meshIndex;
				newModel.materialIndex = meshes[meshIndex]->mMaterialIndex;

				out.push_back(newModel);
			}

			for (unsigned int i = 0; i < node->mNumChildren; i++) {
				if (!loadModels(node->mChildren[i], transform, meshes, out))
					return false;
			}
			return true;
		}*/

		bool SceneImporter::loadMeshes(aiMesh** meshes, unsigned int numMeshes, std::vector<MeshDesc>& meshesOut) {
            meshesOut.clear();
            meshesOut.resize(numMeshes);

            P3D_ASSERT_R(sizeof(glm::vec3) == sizeof(aiVector3D), "Failed to load meshes. Vec3d sizes do not match");
            P3D_ASSERT_R(sizeof(glm::vec4) == sizeof(aiColor4D), "Failed to load meshes. Vec4d sizes do not match");

			for (unsigned int i = 0; i < numMeshes; i++) {
                auto* mesh = meshes[i];
                auto& meshOut = meshesOut[i];

                if (mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
                    //print warning (Mesh is not triangulated, skipping)
                    break;
                }

                if (mesh->HasPositions()) {
                    meshOut.vertices.reset(new glm::vec3[mesh->mNumVertices]);
                    meshOut.verticesSize = mesh->mNumVertices;
                    memcpy(meshOut.vertices.get(), mesh->mVertices, sizeof(glm::vec3) * mesh->mNumVertices);
                }

                if (mesh->HasFaces()) {
                    //ensure aiProcess_Triangulate flag is set. Otherwise, faces are not guaranteed to have 3 indices
                    meshOut.indicesSize = mesh->mNumFaces * 3;
                    meshOut.indices.reset(new unsigned int[meshOut.indicesSize]);

                    unsigned int ii = 0;
                    for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
                        const aiFace& tempFace = mesh->mFaces[f];
                        for (unsigned int i = 0; i < 3; i++) {
                            meshOut.indices.get()[ii] = tempFace.mIndices[i];
                            ii++;
                        }
                    }
                }

                if (mesh->HasNormals()) {
                    meshOut.normals.reset(new glm::vec3[mesh->mNumVertices]);
                    memcpy(meshOut.normals.get(), mesh->mNormals, sizeof(glm::vec3) * mesh->mNumVertices);
                }

                if (mesh->GetNumUVChannels() > 1) {
                    //print warning (multiple UV channels detected, can only load channel #1)
                }
                if (mesh->HasTextureCoords(0) && mesh->mNumUVComponents[0] == 2) {
                    meshOut.texCoords.reset(new glm::vec2[mesh->mNumVertices]);
                    for (unsigned int tc = 0; tc < mesh->mNumVertices; tc++) {
                        //only load UV components
                        meshOut.texCoords.get()[tc] = { mesh->mTextureCoords[0]->x, mesh->mTextureCoords[0]->y };
                    }
                } else if (mesh->mNumUVComponents[0] != 2) {
                    //print warning (texture is not 2d. Skipping)
                }
                
                if (mesh->GetNumColorChannels() > 1) {
                    //print warning (multiple UV channels detected, can only load channel #1)
                }
                if (mesh->HasVertexColors(0)) {
                    meshOut.colors.reset(new glm::vec4[mesh->mNumVertices]);
                    memcpy(meshOut.colors.get(), mesh->mColors[0], sizeof(glm::vec4) * mesh->mNumVertices);
                }

                if (mesh->HasTangentsAndBitangents()) {
                    meshOut.tangents.reset(new glm::vec3[mesh->mNumVertices]);
                    memcpy(meshOut.tangents.get(), mesh->mTangents, sizeof(glm::vec3) * mesh->mNumVertices);

                    meshOut.bitangents.reset(new glm::vec3[mesh->mNumVertices]);
                    memcpy(meshOut.bitangents.get(), mesh->mBitangents, sizeof(glm::vec3) * mesh->mNumVertices);
                }

                //TODO load bones
			}
			return true;
		}

		/*bool SceneImporter::loadMaterials(aiMaterial** materials, unsigned int numMaterials, std::string scenePath, std::vector<model::Material>& out) {
			for (unsigned int i = 0; i < numMaterials; i++) {
				model::Material newMaterial = {};
				if (!loadMaterial(materials[i], scenePath, newMaterial))
					return false;

				out.push_back(newMaterial);
			}
			return true;
		}

		bool SceneImporter::loadMaterial(aiMaterial* material, std::string scenePath, model::Material& out) {
			{
				aiColor3D val;
				if (material->Get(AI_MATKEY_COLOR_DIFFUSE, val) == AI_SUCCESS)
					out.diffuseColor = { val.r, val.g, val.b };
				else
					out.diffuseColor = { 0.0f, 0.0f, 0.0f };

				if (material->Get(AI_MATKEY_COLOR_AMBIENT, val) == AI_SUCCESS)
					out.ambientColor = { val.r, val.g, val.b };
				else
					out.ambientColor = { 0.0f, 0.0f, 0.0f };

				if (material->Get(AI_MATKEY_COLOR_SPECULAR, val) == AI_SUCCESS)
					out.specularColor = { val.r, val.g, val.b };
				else
					out.specularColor = { 0.0f, 0.0f, 0.0f };

				if (material->Get(AI_MATKEY_COLOR_TRANSPARENT, val) == AI_SUCCESS)
					out.transparentColor = { val.r, val.g, val.b };
				else
					out.transparentColor = { 0.0f, 0.0f, 0.0f };

				if (material->Get(AI_MATKEY_COLOR_EMISSIVE, val) == AI_SUCCESS)
					out.emissiveColor = { val.r, val.g, val.b };
				else
					out.emissiveColor = { 0.0f, 0.0f, 0.0f };
			}

			{
				float val;
				if (material->Get(AI_MATKEY_SHININESS, val) == AI_SUCCESS)
					out.shininess = val;
				else
					out.shininess = 0.0f;

				if (material->Get(AI_MATKEY_SHININESS_STRENGTH, val) == AI_SUCCESS)
					out.shininessStr = val;
				else
					out.shininessStr = 0.0f;

				if (material->Get(AI_MATKEY_OPACITY, val) == AI_SUCCESS)
					out.opacity = val;
				else
					out.opacity = 1.0f;
			}

			{
				int val;
				if (material->Get(AI_MATKEY_ENABLE_WIREFRAME, val) == AI_SUCCESS)
					out.wireframe = val == 0 ? false : true;
				else
					out.wireframe = false;

				if (material->Get(AI_MATKEY_BLEND_FUNC, val) == AI_SUCCESS) {
					switch (val) {
					case aiBlendMode_Default:
						out.blendMode = BLEND_MODE_DEFAULT;
						break;
					case aiBlendMode_Additive:
						out.blendMode = BLEND_MODE_ADDITIVE;
						break;
					default:
						return false;
					}
				} else {
					out.blendMode = BLEND_MODE_NONE;
				}
			}

			if (!loadTexture2D(material, aiTextureType_DIFFUSE, scenePath, out.hasDiffuseTex, out.diffuseTex))
				return false;
			if (!loadTexture2D(material, aiTextureType_AMBIENT, scenePath, out.hasAmbientTex, out.ambientTex))
				return false;
			if (!loadTexture2D(material, aiTextureType_SPECULAR, scenePath, out.hasSpecularTex, out.specularTex))
				return false;
			if (!loadTexture2D(material, aiTextureType_NORMALS, scenePath, out.hasNormalTex, out.normalTex))
				return false;
			if (!loadTexture2D(material, aiTextureType_SHININESS, scenePath, out.hasShininessTex, out.shininessTex))
				return false;
			if (!loadTexture2D(material, aiTextureType_OPACITY, scenePath, out.hasOpacityTex, out.opacityTex))
				return false;
			if (!loadTexture2D(material, aiTextureType_EMISSIVE, scenePath, out.hasEmissionTex, out.emissionTex))
				return false;

			return true;
		}

		bool SceneImporter::loadTexture2D(aiMaterial* material, aiTextureType type, std::string scenePathStr, bool& enabled, model::Texture2D& out) {
			enabled = false;
			
			aiString texPathStr;
			aiTextureMapping mapping;
			aiTextureMapMode mapMode[3]; 

			if (material->GetTextureCount(type) > 0
				&& material->GetTexture(type, 0, &texPathStr, &mapping, nullptr, nullptr, nullptr, mapMode) == AI_SUCCESS) {

				if (mapping != aiTextureMapping_UV)
					return false;

				//supports only one map mode per texture
				if (mapMode[0] != mapMode[1])
					return false;

				std::experimental::filesystem::path texP(texPathStr.data);
				std::experimental::filesystem::path fullTexP = texP.is_absolute() ?
					texP : std::experimental::filesystem::path(scenePathStr).append(texP);
				
				if (!std::experimental::filesystem::exists(fullTexP))
					return false;

				out.source = DATA_SOURCE::FILE;
				out.path = fullTexP.string();

				if (!loadTextureMapMode(mapMode[0], out.texMapMode))
						return false;

				enabled = true;
			}
			return true;
		}

		bool SceneImporter::loadTextureMapMode(aiTextureMapMode in, TEXTURE_MAP_MODE& out) {
			switch (in) {
			case aiTextureMapMode_Wrap:
				out = TEXTURE_MAP_MODE::WRAP;
				break;
			case aiTextureMapMode_Clamp:
				out = TEXTURE_MAP_MODE::CLAMP;
				break;
			case aiTextureMapMode_Mirror:
				out = TEXTURE_MAP_MODE::MIRROR;
				break;
			default:
				return false;
			}
			return true;
		}

		bool SceneImporter::loadLights(aiLight** lights, unsigned int numLights, std::vector<model::Light>& out) {
			for (unsigned int i = 0; i < numLights; i++) {
				model::Light newLight = {};
				if (!loadLight(lights[i], newLight))
					return false;
				out.push_back(newLight);
			}
			return true;
		}

		bool SceneImporter::loadLight(aiLight* in, model::Light& out) {
			out.diffuse = glm::make_vec3(&in->mColorDiffuse[0]);
			out.specular = glm::make_vec3(&in->mColorSpecular[0]);
			out.position = glm::make_vec3(&in->mPosition[0]);

			switch (in->mType) {
			case aiLightSource_POINT: {
				out.type = LIGHT_TYPE::POINT;
				out.data.point.attenuation = { in->mAttenuationConstant, in->mAttenuationLinear, in->mAttenuationQuadratic };
				out.data.point.range = 10.0f;

			}break;
			case aiLightSource_DIRECTIONAL: {
				return false;
			}break;
			case aiLightSource_SPOT: {
				return false;
			}break;
			default:
				return false;
			}
			return true;
		}*/
	}
}