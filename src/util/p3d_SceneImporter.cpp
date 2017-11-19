#include "p3d_SceneImporter.h"
#include "../p3d_ResourceModel.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/gtc/type_ptr.hpp>

#include <experimental/filesystem>

namespace p3d {
	namespace util {
		SceneImporter::SceneImporter() {
		}

		SceneImporter::~SceneImporter() {
		}

		bool SceneImporter::import(std::string pathStr, model::Scene& out) {
			std::experimental::filesystem::path scenePath(pathStr);
			if (!std::experimental::filesystem::exists(scenePath))
				return false;
			scenePath = scenePath.remove_filename();

			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(pathStr,
				aiProcess_CalcTangentSpace
				| aiProcess_Triangulate
				| aiProcess_JoinIdenticalVertices
				| aiProcess_SortByPType
				| aiProcess_GenSmoothNormals
				| aiProcess_PreTransformVertices
				| aiProcess_ValidateDataStructure
				| aiProcess_RemoveRedundantMaterials
			);

			if (!scene) {
				//importer.GetErrorString();
				return false;
			}
			
			aiMatrix4x4 identity;
			if (!loadModels(scene->mRootNode, identity, scene->mMeshes, out.models))
				return false;

			if (!scene->HasMeshes() || !loadMeshes(scene->mMeshes, scene->mNumMeshes, out.meshs))
				return false;

			if (!scene->HasMaterials() || !loadMaterials(scene->mMaterials, scene->mNumMaterials, scenePath.string(), out.materials))
				return false;

			return true;
		}

		bool SceneImporter::loadModels(aiNode* node, aiMatrix4x4 parentTransform, aiMesh** meshes, std::vector<model::Model>& out) {
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
		}

		bool SceneImporter::loadMeshes(aiMesh** meshes, unsigned int numMeshes, std::vector<model::Mesh>& out) {
			for (unsigned int i = 0; i < numMeshes; i++) {
				model::Mesh newMesh = {};
				if (!meshes[i]->HasPositions() || !loadVertices(meshes[i], newMesh))
					return false;

				if (!meshes[i]->HasFaces() || !loadIndices(meshes[i], newMesh))
					return false;

				out.push_back(newMesh);
			}
			return true;
		}

		bool SceneImporter::loadVertices(aiMesh* mesh, model::Mesh& out) {
			if (mesh->HasTextureCoords(0)) {
				out.vertexType = VERTEX_TYPE::TEXTURED;

				std::vector<Vertex_Textured>* vertices = new std::vector<Vertex_Textured>();
				for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
					const aiVector3D pos = mesh->mVertices[i];
					const aiVector3D normal = mesh->mNormals[i];
					const aiVector3D texCoord = mesh->mTextureCoords[0][i];

					Vertex_Textured v = {
						{ pos.x, pos.y, pos.z },
						{ normal.x, normal.y, normal.z },
						{ texCoord.x, texCoord.y}
					};
					vertices->push_back(v);
				}
				out.vertices.textured = vertices;

			} else {
				out.vertexType = VERTEX_TYPE::UNTEXTURED;

				std::vector<Vertex_Untextured>* vertices = new std::vector<Vertex_Untextured>();
				for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
					const aiVector3D pos = mesh->mVertices[i];

					Vertex_Untextured v = {
						{ pos.x, pos.y, pos.z }
					};
					vertices->push_back(v);
				}
				out.vertices.untextured = vertices;
			}

			return true;
		}

		bool SceneImporter::loadIndices(aiMesh* mesh, model::Mesh& out) {
			out.indices = new std::vector<unsigned int>();

			unsigned int numIndices = mesh->mFaces[0].mNumIndices;
			switch (numIndices) {
			case 2:
				out.topology = PRIMITIVE_TOPOLOGY_TYPE::LINE_LIST;
				break;
			case 3:
				out.topology = PRIMITIVE_TOPOLOGY_TYPE::TRIANGLE_LIST;
				break;
			default:
				return false;
			}

			for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
				const aiFace& tempFace = mesh->mFaces[f];
				for (unsigned int i = 0; i < numIndices; i++) {
					out.indices->push_back(tempFace.mIndices[i]);
				}
			}

			return true;
		}

		bool SceneImporter::loadMaterials(aiMaterial** materials, unsigned int numMaterials, std::string scenePath, std::vector<model::Material>& out) {
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
	}
}