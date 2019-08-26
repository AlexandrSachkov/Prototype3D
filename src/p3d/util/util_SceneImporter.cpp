#include "util_SceneImporter.h"
#include "../assert.h"
#include "util_DDSTextureLoader.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/gtc/type_ptr.hpp>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <experimental/filesystem>

namespace p3d {
	namespace util {
		bool SceneImporter::import(const std::string& path, const std::string& userTexDir, SceneI* sceneOut, SceneGraphI* sceneGraph) {
            P3D_ASSERT_R(sceneOut, "Failed to import scene. Scene is null");

			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path,
				aiProcess_CalcTangentSpace
				| aiProcess_Triangulate
				| aiProcess_JoinIdenticalVertices
				| aiProcess_SortByPType
                | aiProcess_OptimizeMeshes
				| aiProcess_GenSmoothNormals
				| aiProcess_ValidateDataStructure
				| aiProcess_RemoveRedundantMaterials
			);

            P3D_ASSERT_R(scene, importer.GetErrorString());
			
            std::vector<HMesh> meshes;
            std::vector<HMaterial> materials;

            if (scene->HasMeshes()) {
                loadMeshes(scene->mMeshes, scene->mNumMeshes, sceneOut, meshes);
            }
            if (scene->HasMaterials()) {
                loadMaterials(scene->mMaterials, scene->mNumMaterials, sceneOut, path, userTexDir, materials);
            }
            
			/*if (scene->HasLights() && !loadLights(scene->mLights, scene->mNumLights, out.lights))
				return false;*/

            loadModels(scene->mRootNode, aiMatrix4x4(), scene->mMeshes, meshes, materials, sceneOut);

			return true;
		}

		void SceneImporter::loadModels(
            aiNode* srcNode,
            aiMatrix4x4 parentTransform,
            aiMesh** srcMeshes,
            const std::vector<HMesh>& meshes,
            const std::vector<HMaterial>& materials,
            SceneI* scene
        ) {
			aiMatrix4x4 transform = parentTransform * srcNode->mTransformation;
			for (unsigned int i = 0; i < srcNode->mNumMeshes; i++) {
                ModelDesc desc;
                desc.transform = glm::make_mat4x4((const float*)(&transform));
                unsigned int srcMeshIndex = srcNode->mMeshes[i];
                desc.mesh = meshes[srcMeshIndex];
                desc.material = materials[srcMeshes[srcMeshIndex]->mMaterialIndex];
				
                HModel hmodel = scene->create(desc);
                P3D_WARNING(!hmodel.isValid(), "Failed to create scene model");
			}

			for (unsigned int i = 0; i < srcNode->mNumChildren; i++) {
                loadModels(srcNode->mChildren[i], transform, srcMeshes, meshes, materials, scene);
			}
		}

		bool SceneImporter::loadMeshes(aiMesh** meshes, unsigned int numMeshes, SceneI* scene, std::vector<HMesh>& meshesOut) {
            meshesOut.clear();
            meshesOut.resize(numMeshes);

            P3D_ASSERT_R(sizeof(glm::vec3) == sizeof(aiVector3D), "Failed to load meshes. Vec3d sizes do not match");
            P3D_ASSERT_R(sizeof(glm::vec4) == sizeof(aiColor4D), "Failed to load meshes. Vec4d sizes do not match");

			for (unsigned int i = 0; i < numMeshes; i++) {
                MeshDesc desc;
                if (!loadMesh(meshes[i], desc)) {
                    P3D_ERROR_PRINT("Failed to load mesh #" + std::to_string(i));
                    continue;
                }
                
                meshesOut[i] = scene->create(desc);
                P3D_WARNING(!meshesOut[i].isValid(), "Failed to create scene mesh");
			}
			return true;
		}

        bool SceneImporter::loadMesh(aiMesh* mesh, MeshDesc& meshOut) {
            P3D_ASSERT_R(mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE, "Mesh is not triangulated");

            //assimp converts to triangle list upon import
            meshOut.topology = P3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

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

            P3D_WARNING(mesh->GetNumUVChannels() > 1, 
                "Mesh contains " + std::to_string(mesh->GetNumUVChannels()) + " UV channels. Only the first channel will be loaded");

            if (mesh->HasTextureCoords(0)) {
                meshOut.texCoords.reset(new glm::vec2[mesh->mNumVertices]);
                for (unsigned int tc = 0; tc < mesh->mNumVertices; tc++) {
                    //only load UV components
                    meshOut.texCoords.get()[tc] = { mesh->mTextureCoords[0]->x, mesh->mTextureCoords[0]->y };
                }
            }

            P3D_WARNING(mesh->GetNumColorChannels() > 1,
                "Mesh contains " + std::to_string(mesh->GetNumColorChannels()) + " color channels. Only the first channel will be loaded");

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
            return true;
        }

		bool SceneImporter::loadMaterials(
            aiMaterial** materials, 
            unsigned int numMaterials, 
            SceneI* scene,
            const std::string& scenePath,
            const std::string& userTexDir,
            std::vector<HMaterial>& materialsOut
        ) {
            materialsOut.clear();
            materialsOut.resize(numMaterials);

            std::unordered_map<std::string, HTexture2dArr> texture2dMap; //for texture reuse
			for (unsigned int i = 0; i < numMaterials; i++) {
                MaterialDesc desc;
                if (!loadMaterial(materials[i], texture2dMap, scene, scenePath, userTexDir, desc)) {
                    P3D_ERROR_PRINT("Failed to load material #" + std::to_string(i));
                    continue;
                }
                
                materialsOut[i] = scene->create(desc);
                P3D_WARNING(!materialsOut[i].isValid(), "Failed to create scene material");
			}
			return true;
		}

		bool SceneImporter::loadMaterial(
            aiMaterial* material, 
            std::unordered_map<std::string, HTexture2dArr>& texMap, 
            SceneI* scene, 
            const std::string& scenePath,
            const std::string& userTexDir,
            MaterialDesc& out
        ) {
            aiString name;
            material->Get(AI_MATKEY_NAME, name);

			{
				aiColor3D val;
                if (material->Get(AI_MATKEY_COLOR_DIFFUSE, val) == AI_SUCCESS) {
                    out.diffuseColor = { val.r, val.g, val.b };
                }
                if (material->Get(AI_MATKEY_COLOR_AMBIENT, val) == AI_SUCCESS) {
                    out.ambientColor = { val.r, val.g, val.b };
                }
                if (material->Get(AI_MATKEY_COLOR_SPECULAR, val) == AI_SUCCESS) {
                    out.specularColor = { val.r, val.g, val.b };
                }
                /*if (material->Get(AI_MATKEY_COLOR_TRANSPARENT, val) == AI_SUCCESS) {
                    out.transparencyColor = { val.r, val.g, val.b };
                }*/

                // TODO load emission and other params
                /*if (material->Get(AI_MATKEY_COLOR_EMISSIVE, val) == AI_SUCCESS) {
                    out.emissionColor = { val.r, val.g, val.b };
                }*/
			}

			{
				float val;
                if (material->Get(AI_MATKEY_SHININESS, val) == AI_SUCCESS) {
                    out.shininess = val;
                }
                if (material->Get(AI_MATKEY_SHININESS_STRENGTH, val) == AI_SUCCESS) {
                    out.shininessStrength = val;
                }
                /*if (material->Get(AI_MATKEY_OPACITY, val) == AI_SUCCESS) {
                    out.opacity = val;
                }*/
			}

			{
				int val;
                if (material->Get(AI_MATKEY_ENABLE_WIREFRAME, val) == AI_SUCCESS) {
                    out.wireframe = val == 0 ? false : true;
                }

                //TODO do we need blend mode?
				/*if (material->Get(AI_MATKEY_BLEND_FUNC, val) == AI_SUCCESS) {
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
				}*/
			}

            P3D_WARNING(!loadTexture2D(material, aiTextureType_DIFFUSE, texMap, scene, scenePath, userTexDir, out.diffuseTex, out.diffuseMapMode),
                "Failed to load diffuse texture of " + std::string(name.C_Str()));

            if (!loadTexture2D(material, aiTextureType_NORMALS, texMap, scene, scenePath, userTexDir, out.normalTex, out.normalMapMode)) {
                P3D_WARNING(!loadTexture2D(material, aiTextureType_HEIGHT, texMap, scene, scenePath, userTexDir, out.normalTex, out.normalMapMode),
                    "Failed to load normal texture of " + std::string(name.C_Str()));
            }
                    
            //TODO load other textures
			/*if (!loadTexture2D(material, aiTextureType_AMBIENT, scenePath, out.hasAmbientTex, out.ambientTex))
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
                */
			return true;
		}

		bool SceneImporter::loadTexture2D(
            aiMaterial* material,
            aiTextureType type,
            std::unordered_map<std::string, HTexture2dArr>& texMap,
            SceneI* scene,
            const std::string& scenePath,
            const std::string& userTexDir,
            HTexture2dArr& texOut,
            P3D_TEX_MAP_MODE& mapModeOut
        ) {
            aiString texPathStr;
            aiTextureMapMode mapMode[3];

            P3D_ASSERT_R(material->GetTextureCount(type) > 0 &&
                material->GetTexture(type, 0, &texPathStr, nullptr, nullptr, nullptr, nullptr, mapMode) == AI_SUCCESS,
                "Unable to retrieve texture info");

            P3D_WARNING(material->GetTextureCount(type) > 1,
                "Multiple texture of same type detected. Able to load only the first one");

            std::string texFinalPath;
            P3D_ASSERT_R(findTexturePath(scenePath, userTexDir, texPathStr.C_Str(), texFinalPath), "Unable to find texture");

            P3D_ASSERT_R(mapMode[0] == mapMode[1], "Different map modes per UV channels not supported");

            P3D_WARNING(!loadTextureMapMode(mapMode[0], mapModeOut), "Failed to load texture mode. Using default");

            auto texIt = texMap.find(texFinalPath);
            if (texIt != texMap.end()) {
                //texture already loaded, reusing
                texOut = texIt->second;
                return true;
            }
                
            TextureDesc texDesc;
            texDesc.path = texFinalPath;
            P3D_ASSERT_R(DDSTextureLoader::loadDDSTextureFromFile(texFinalPath, texDesc), "DDSTextureLoader failed to load texture");

            texDesc.usageFlag = P3D_USAGE::P3D_USAGE_GPU_R;
            texDesc.bindFlags = { P3D_BIND_SHADER_RESOURCE };
            texDesc.generateMipMaps = true;

            texOut = scene->create(texDesc);
            P3D_ASSERT_R(texOut.isValid(), "Scene failed to create texture");

            texMap.emplace(texFinalPath, texOut);
			return true;
		}

		bool SceneImporter::loadTextureMapMode(aiTextureMapMode in, P3D_TEX_MAP_MODE& out) {
			switch (in) {
			case aiTextureMapMode_Wrap:
				out = P3D_TEX_MAP_MODE::P3D_TEX_MAP_WRAP;
				break;
			case aiTextureMapMode_Clamp:
				out = P3D_TEX_MAP_MODE::P3D_TEX_MAP_CLAMP;
				break;
			case aiTextureMapMode_Mirror:
				out = P3D_TEX_MAP_MODE::P3D_TEX_MAP_MIRROR;
				break;
			default:
				return false;
			}
			return true;
		}

        bool SceneImporter::findTexturePath(
            const std::string& scenePathStr,
            const std::string& userTexDirStr,
            const std::string& bakedTexPathStr,
            std::string& finalPath
        ) {
            std::experimental::filesystem::path bakedTexPath(bakedTexPathStr);
            bakedTexPath.replace_extension(".dds");
            std::experimental::filesystem::path texFileName = bakedTexPath.filename();

            std::experimental::filesystem::path userTexDir(userTexDirStr);
            std::experimental::filesystem::path userTexPath = userTexDir.append(texFileName);
            if (std::experimental::filesystem::exists(userTexPath)) {
                finalPath = userTexPath.string();
                return true;
            }

            if (std::experimental::filesystem::exists(bakedTexPath)) {
                finalPath = bakedTexPath.string();
                return true;
            }

            if (bakedTexPath.is_relative()) {
                std::experimental::filesystem::path bakedRelativeToScenePath =
                    std::experimental::filesystem::path(scenePathStr).parent_path().append(bakedTexPath);
                if (std::experimental::filesystem::exists(bakedRelativeToScenePath)) {
                    finalPath = bakedRelativeToScenePath.string();
                    return true;
                }
            }

            std::experimental::filesystem::path sceneTexPath = 
                std::experimental::filesystem::path(scenePathStr).parent_path().append(texFileName);
            if (std::experimental::filesystem::exists(sceneTexPath)) {
                finalPath = sceneTexPath.string();
                return true;
            }

            return false;
        }

		/*bool SceneImporter::loadLights(aiLight** lights, unsigned int numLights, std::vector<model::Light>& out) {
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