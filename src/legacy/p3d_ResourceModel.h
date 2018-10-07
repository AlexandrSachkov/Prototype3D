#pragma once

#include "glm/glm.hpp"

#include <vector>
#include <string>

namespace p3d
{
	enum LIGHT_TYPE
	{
		DIRECTIONAL,
		POINT,
		SPOT
	};

	enum VERTEX_TYPE
	{
		TEXTURED,
		UNTEXTURED
	};

	enum PRIMITIVE_TOPOLOGY_TYPE
	{
		LINE_LIST,
		TRIANGLE_LIST
	};

	enum DATA_SOURCE
	{
		FILE,
		MEMORY
	};

	enum TEXTURE_MAP_MODE
	{
		WRAP,
		MIRROR,
		CLAMP,

		NUM_TEXTURE_MAP_MODES
	};

	enum BLEND_MODE
	{
		BLEND_MODE_NONE,
		BLEND_MODE_ADDITIVE,
		BLEND_MODE_DEFAULT
	};

	struct Vertex_Untextured {
		float position[3];
	};

	struct Vertex_Textured {
		float position[3];
		float normal[3];
		//float tangent[3];
		//float bitangent[3];
		float uv[2];
	};

	namespace model
	{

		struct DirectionalLight
		{
			glm::vec3 direction;
		};

		struct PointLight
		{
			glm::vec3 attenuation;
			float range;
		};

		struct SpotLight
		{
			glm::vec3 direction;
			glm::vec3 attenuation;
			float range;
			float spot;
		};

		struct Light
		{
			LIGHT_TYPE type;
			glm::vec3 position;
			glm::vec3 diffuse;
			glm::vec3 specular;
			union Data{
				Data() {}
				DirectionalLight direct;
				PointLight point;
				SpotLight spot;
			} data;
		};

		struct Texture2D
		{
			DATA_SOURCE source;
			TEXTURE_MAP_MODE texMapMode;

			std::string path;
			std::vector<char>* data;
		};

		struct Mesh
		{
			PRIMITIVE_TOPOLOGY_TYPE topology;
			VERTEX_TYPE vertexType;

			union {
				std::vector<Vertex_Untextured>* untextured;
				std::vector<Vertex_Textured>* textured;
			} vertices;
			
			std::vector<unsigned int>* indices;
		};

		struct Material
		{
			glm::vec3 diffuseColor;
			glm::vec3 ambientColor;
			glm::vec3 specularColor;
			glm::vec3 transparentColor;
			glm::vec3 emissiveColor;

			Texture2D diffuseTex;
			Texture2D ambientTex;
			Texture2D specularTex;
			Texture2D normalTex;
			Texture2D shininessTex;
			Texture2D opacityTex;
			Texture2D emissionTex;
			
			float shininess;
			float shininessStr;
			float opacity;

			BLEND_MODE blendMode;
			bool wireframe;

			bool hasDiffuseTex;
			bool hasAmbientTex;
			bool hasSpecularTex;
			bool hasNormalTex;
			bool hasShininessTex;
			bool hasOpacityTex;
			bool hasEmissionTex;
		};

		struct Model
		{
			glm::mat4x4 transform;
			unsigned int meshIndex;
			unsigned int materialIndex;
		};

		struct Scene
		{
			std::vector<Model> models;
			std::vector<Mesh> meshs;
			std::vector<Material> materials;
			std::vector<Light> lights;
		};
	}
}