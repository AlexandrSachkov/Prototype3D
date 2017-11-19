#pragma once

#include "NL_RenderingResources.h"
#include "NL_Console.h"

#pragma warning( push )
#pragma warning( disable : 4005)

#include <assimp\Importer.hpp>
#include <assimp\scene.h> 
#include <assimp\postprocess.h>  
#include <assimp\Logger.hpp>
#include <assimp\LogStream.hpp>
#include <assimp\DefaultLogger.hpp>

#pragma warning( pop ) 

#include <d3d11.h>
#include <DirectXMath.h>

#include <string>
#include <vector>
#include <memory>


namespace NLE
{
	namespace GRAPHICS
	{
		class Scene;
	}
	namespace IMPORTER
	{
		class TextureLoader;

		class AssimpLogStream : public Assimp::LogStream
		{
		public:
			AssimpLogStream(){};
			~AssimpLogStream(){};

			void write(const char* message)
			{
				std::string msg(message);
				CONSOLE::out(CONSOLE::DEBUG, "Assimp error: " + msg);
			}
		};

		class AssetImporter
		{
		public:
			AssetImporter();
			~AssetImporter();

			void importScene(
				ID3D11Device* d3dDevice, 
				std::wstring& path, 
				GRAPHICS::Scene& scene
				);
			void setTextureResourcePath(std::wstring path);

		private:
			GRAPHICS::RESOURCES::Mesh* loadMeshes(ID3D11Device* d3dDevice, const aiScene* scene);
			void loadVertices(aiMesh* mesh, GRAPHICS::RESOURCES::Vertex*& vertexArr, unsigned int& streamLength);
			void loadIndices(aiMesh* mesh, GRAPHICS::RESOURCES::Index*& indexArr, unsigned int& length, D3D_PRIMITIVE_TOPOLOGY& primitiveTopology);
			void loadLights(ID3D11Device* d3dDevice, GRAPHICS::Scene& outScene, const aiScene* scene);

			void loadMaterialBuffer(aiMaterial* material, GRAPHICS::RESOURCES::MaterialBuffer& materialBuffer);
			void loadMaterialParams(aiMaterial* material, GRAPHICS::RESOURCES::Material& nleMaterial);
			void loadMaterialTextures(ID3D11Device* d3dDevice, aiMaterial* material, GRAPHICS::RESOURCES::Material& nleMaterial);
			GRAPHICS::RESOURCES::Material* loadMaterials(ID3D11Device* d3dDevice, const aiScene* scene);

			void nextNode(
				ID3D11Device* d3dDevice,
				const aiScene* scene,
				aiNode* node,
				aiMatrix4x4 accTransform,
				GRAPHICS::RESOURCES::Mesh* meshArr,
				GRAPHICS::RESOURCES::Material* materialArr,
				GRAPHICS::Scene& outScene
				);

			void assembleAsset(
				ID3D11Device* d3dDevice,
				const aiScene* scene,
				unsigned int meshIndex,
				aiMatrix4x4 transform,
				GRAPHICS::RESOURCES::Mesh* meshArr,
				GRAPHICS::RESOURCES::Material* materialArr,
				GRAPHICS::Scene& outScene
				);

			std::wstring generateTextureResourcePath(aiString textureResourcePath);

			void printFloat4x4(DirectX::XMFLOAT4X4& matrix);
			void printFloat4x4(aiMatrix4x4& matrix);

			std::wstring _textureResourcePath;
		};
	}
}