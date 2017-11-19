#pragma once

#include "../p3d_ResourceModel.h"

#include <Windows.h>
#include <d3d11.h>

#include "glm/glm.hpp"

#include <array>

namespace p3d {
	namespace d3d11 {
		namespace runtime {

			struct VertexShader {
				ID3D11VertexShader* shader;
				ID3DBlob* blob;
			};

			struct PixelShader {
				ID3D11PixelShader* shader;
				ID3DBlob* blob;
			};

			__declspec(align(16))
			struct CameraData {
				glm::mat4x4 ViewProj;
				glm::vec3 eye;
			};

			struct Texture2D
			{
				ID3D11Texture2D* texture;
				ID3D11ShaderResourceView* view;
				TEXTURE_MAP_MODE texMapMode;
			};

			struct Mesh
			{
				D3D11_PRIMITIVE_TOPOLOGY topology;
				VERTEX_TYPE vertexType;

				ID3D11Buffer* vertexBuff;
				unsigned int numVertices;
				ID3D11Buffer* indexBuff;
				unsigned int numIndices;
			};

			__declspec(align(16))
			struct MaterialData
			{
				glm::vec4 diffuseColor;
				glm::vec4 ambientColor;
				glm::vec4 specularColor;
				glm::vec4 transparentColor;
				glm::vec4 emissiveColor;

				float shininess;
				float shininessStr;
				float opacity;
				float _b;

				bool hasDiffuseTex;
				bool hasAmbientTex;
				bool hasSpecularTex;
				bool hasNormalTex;
				bool hasShininessTex;
				bool hasOpacityTex;
				bool hasEmissionTex;
			};

			struct Material
			{				
				MaterialData matData;

				Texture2D diffuseTex;
				Texture2D ambientTex;
				Texture2D specularTex;
				Texture2D normalTex;
				Texture2D shininessTex;
				Texture2D opacityTex;
				Texture2D emissionTex;

				BLEND_MODE blendMode;
				bool wireframe;
			};

			__declspec(align(16))
			struct TransformData
			{
				glm::mat4x4 world;
				glm::mat4x4 worldInvTrans;
				glm::mat4x4 wvp;
			};

			struct Model
			{
				glm::mat4x4 world;
				glm::mat4x4 worldInvTrans;
				unsigned int meshIndex;
				unsigned int materialIndex;
			};

			struct Scene
			{
				std::vector<Model> models;
				std::vector<Mesh> meshs;
				std::vector<Material> materials;
			};
		}
	}
}