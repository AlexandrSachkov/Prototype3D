#include "p3d_RenderingEngine.h"
#include "internal/NL_D3D11Utility.h"
#include "internal/p3d_d3d11_RuntimeModel.h"
#include "internal/p3d_d3d11_Utility.h"
#include "internal/p3d_Math.h"
#include "p3d_Camera.h"

#include <imgui.h>

#include <Windows.h>
#include <d3d11.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "DDSTextureLoader/DDSTextureLoader.h"
#include "WICTextureLoader/WICTextureLoader.h"

#include <experimental/filesystem>

namespace p3d {
	namespace d3d11 {
		RenderingEngine::RenderingEngine() :
			_renderUiCallback([](void* device, void* deviceContext) {}), _scene(new runtime::Scene()) {
		}

		RenderingEngine::~RenderingEngine() {
		}

		bool RenderingEngine::initialize(void* windowHandle, int width, int height, bool fullscreen) {
			if (!D3D11Utility::createDeviceAndSwapChain(
				static_cast<HWND>(windowHandle),
				width,
				height,
				fullscreen,
				_d3dDevice,
				_swapChain,
				_deviceContext))
				return false;

			if (!D3D11Utility::createBackBufferRenderTargetView(_d3dDevice, _swapChain, _backBufferRenderTargetView))
				return false;
			if (!D3D11Utility::createDepthStencilView(_d3dDevice, width, height, _depthStencilView))
				return false;

			if (!util::loadVertexShader(_d3dDevice, "D:\\Repositories\\Prototype3D\\src\\internal\\shaders\\untextured_VS.hlsl", _untexturedVS))
				return false;
			if (!util::loadPixelShader(_d3dDevice, "D:\\Repositories\\Prototype3D\\src\\internal\\shaders\\untextured_PS.hlsl", _untexturedPS))
				return false;
			if (!util::loadVertexShader(_d3dDevice, "D:\\Repositories\\Prototype3D\\src\\internal\\shaders\\textured_VS.hlsl", _texturedVS))
				return false;
			if (!util::loadPixelShader(_d3dDevice, "D:\\Repositories\\Prototype3D\\src\\internal\\shaders\\textured_PS.hlsl", _texturedPS))
				return false;

			D3D11_INPUT_ELEMENT_DESC untexturedILDesc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			D3D11_INPUT_ELEMENT_DESC texturedILDesc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			if (!util::createInputLayout(_d3dDevice, untexturedILDesc, ARRAYSIZE(untexturedILDesc), _untexturedVS, _untexturedIL))
				return false;
			if (!util::createInputLayout(_d3dDevice, texturedILDesc, ARRAYSIZE(texturedILDesc), _texturedVS, _texturedIL))
				return false;

			if (!D3D11Utility::createBlendStates(_d3dDevice, false, false, 1, false, _noBlendState))
				return false;
			if (!D3D11Utility::createRasterizerState(_d3dDevice, D3D11_CULL_BACK, D3D11_FILL_SOLID, _backFaceCull))
				return false;
			if (!D3D11Utility::createRasterizerState(_d3dDevice, D3D11_CULL_FRONT, D3D11_FILL_SOLID, _frontFaceCull))
				return false;
			if (!D3D11Utility::createRasterizerState(_d3dDevice, D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, _wireframeMode))
				return false;

			for (unsigned int i = 0; i < _samplerStates.size(); i++) {
				if (!util::createTextureSamplerState(_d3dDevice, util::convertTextMapMode((TEXTURE_MAP_MODE)i), _samplerStates[i]))
					return false;
			}

			runtime::MaterialData matData = {};
			if (!util::createConstBuffDynamic(_d3dDevice, &matData, sizeof(runtime::MaterialData), _materialBuff))
				return false;

			runtime::TransformData transform = {};
			if (!util::createConstBuffDynamic(_d3dDevice, &transform, sizeof(runtime::TransformData), _transformBuff))
				return false;

			_deviceContext->OMSetRenderTargets(1, &_backBufferRenderTargetView, _depthStencilView);

			util::setViewPort(_deviceContext, 1, { 0,0 }, { width, height }, { 0.0f, 1.0f });
			_projection = math::perspectiveDX(glm::radians(45.0f), (float)width / (float)height, 1.0f, 1000.f);

			return true;
		}

		void* RenderingEngine::getDevice() {
			return _d3dDevice;
		}

		void* RenderingEngine::getDeviceContext() {
			return _deviceContext;
		}

		void RenderingEngine::attachUiRenderingCallback(std::function<void(void* device, void* deviceContext)> callback) {
			_renderUiCallback = callback;
		}

		void RenderingEngine::render(double deltaT, const Camera& camera) {
			float backgroundColor[] = { 0.0f, 0.0f, 0.0f };
			_deviceContext->ClearRenderTargetView(_backBufferRenderTargetView, backgroundColor);
			_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			for (auto model : _scene->models) {
				auto mesh = _scene->meshs[model.meshIndex];
				auto material = _scene->materials[model.materialIndex];

				switch (mesh.vertexType) {
				case VERTEX_TYPE::UNTEXTURED: {
					_deviceContext->VSSetShader(_untexturedVS.shader, nullptr, 0);
					_deviceContext->PSSetShader(_untexturedPS.shader, nullptr, 0);
					_deviceContext->IASetInputLayout(_untexturedIL);
					UINT stride = sizeof(Vertex_Untextured), offset = 0;
					_deviceContext->IASetVertexBuffers(0, 1, &mesh.vertexBuff, &stride, &offset);
				} break;
				case VERTEX_TYPE::TEXTURED: {
					_deviceContext->VSSetShader(_texturedVS.shader, nullptr, 0);
					_deviceContext->PSSetShader(_texturedPS.shader, nullptr, 0);
					_deviceContext->IASetInputLayout(_texturedIL);
					UINT stride = sizeof(Vertex_Textured), offset = 0;
					_deviceContext->IASetVertexBuffers(0, 1, &mesh.vertexBuff, &stride, &offset);

					if (material.matData.hasDiffuseTex) {
						_deviceContext->PSSetShaderResources(0, 1, &material.diffuseTex.view);
						_deviceContext->PSSetSamplers(0, 1, &_samplerStates[material.diffuseTex.texMapMode]);
					}
					if (material.matData.hasAmbientTex) {
						_deviceContext->PSSetShaderResources(1, 1, &material.ambientTex.view);
						_deviceContext->PSSetSamplers(1, 1, &_samplerStates[material.ambientTex.texMapMode]);
					}
					if (material.matData.hasSpecularTex) {
						_deviceContext->PSSetShaderResources(2, 1, &material.specularTex.view);
						_deviceContext->PSSetSamplers(2, 1, &_samplerStates[material.specularTex.texMapMode]);
					}
					if (material.matData.hasNormalTex) {
						_deviceContext->PSSetShaderResources(3, 1, &material.normalTex.view);
						_deviceContext->PSSetSamplers(3, 1, &_samplerStates[material.normalTex.texMapMode]);
					}
					if (material.matData.hasShininessTex) {
						_deviceContext->PSSetShaderResources(4, 1, &material.shininessTex.view);
						_deviceContext->PSSetSamplers(4, 1, &_samplerStates[material.shininessTex.texMapMode]);
					}
					if (material.matData.hasOpacityTex) {
						_deviceContext->PSSetShaderResources(5, 1, &material.opacityTex.view);
						_deviceContext->PSSetSamplers(5, 1, &_samplerStates[material.opacityTex.texMapMode]);
					}
					if (material.matData.hasEmissionTex) {
						_deviceContext->PSSetShaderResources(6, 1, &material.emissionTex.view);
						_deviceContext->PSSetSamplers(6, 1, &_samplerStates[material.emissionTex.texMapMode]);
					}
				} break;
				default:
					assert(false);
					break;
				}

				glm::mat4x4 translate = glm::translate(model.world, { 0,0,-1 });
				//glm::mat4x4 rotate = glm::rotate(translate, glm::radians(180.0f), { 0, 1, 0 });
				//glm::mat4x4 finTrans = glm::scale(translate, {0.5f, 0.5f, 0.5f});

				glm::mat4x4 wvp = _projection * camera.getView() * translate;
				//glm::mat4x4 wvp = _projection * view * model.world;
				runtime::TransformData transform = { model.world, model.worldInvTrans, wvp };
				util::updateConstBuff(_deviceContext, _transformBuff, &transform, sizeof(transform));
				_deviceContext->VSSetConstantBuffers(0, 1, &_transformBuff);

				util::updateConstBuff(_deviceContext, _materialBuff, &material.matData, sizeof(material.matData));
				_deviceContext->PSSetConstantBuffers(0, 1, &_materialBuff);

				_deviceContext->IASetIndexBuffer(mesh.indexBuff, DXGI_FORMAT_R32_UINT, 0);
				_deviceContext->IASetPrimitiveTopology(mesh.topology);

				float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
				_deviceContext->OMSetBlendState(_noBlendState, blendFactor, 0xffffffff);
				_deviceContext->RSSetState(material.wireframe ? _wireframeMode : _backFaceCull);

				_deviceContext->DrawIndexed(mesh.numIndices, 0, 0);
			}

			_renderUiCallback(_d3dDevice, _deviceContext);
			_swapChain->Present(0, 0);
		}

		bool RenderingEngine::load(const model::Scene& sceneM) {
			//load models
			{
				runtime::Model newModel;
				for (auto model : sceneM.models) {
					newModel.materialIndex = model.materialIndex;
					newModel.meshIndex = model.meshIndex;
					newModel.world = model.transform;
					newModel.worldInvTrans = glm::transpose(glm::inverse(model.transform));
					_scene->models.push_back(newModel);
				}
			}

			//load meshes
			{
				runtime::Mesh newMesh;
				for (auto mesh : sceneM.meshs) {
					newMesh.vertexType = mesh.vertexType;
					newMesh.topology = util::convertTopology(mesh.topology);

					switch (mesh.vertexType) {
					case UNTEXTURED: {
						if (!mesh.vertices.untextured)
							return false;

						auto* vertices = mesh.vertices.untextured;
						if (!util::createVertexBuffStatic(_d3dDevice, vertices->data(), sizeof(Vertex_Untextured), (unsigned int)vertices->size(), newMesh.vertexBuff))
							return false;

						newMesh.numVertices = (unsigned int)vertices->size();
					} break;

					case TEXTURED: {
						if (!mesh.vertices.textured)
							return false;

						auto* vertices = mesh.vertices.textured;
						if (!util::createVertexBuffStatic(_d3dDevice, vertices->data(), sizeof(Vertex_Textured), (unsigned int)vertices->size(), newMesh.vertexBuff))
							return false;

						newMesh.numVertices = (unsigned int)vertices->size();
					} break;
					default:
						return false;
					}
					auto* indices = mesh.indices;
					if (!mesh.indices || !util::createIndexBuffStatic(_d3dDevice, indices->data(), (unsigned int)indices->size(), newMesh.indexBuff))
						return false;

					newMesh.numIndices = (unsigned int)indices->size();

					_scene->meshs.push_back(newMesh);
				}
			}

			//load materials
			{
				runtime::Material newMaterial = {};
				for (auto material : sceneM.materials) {
					newMaterial.matData.ambientColor = glm::vec4(material.ambientColor, 1.0f);
					newMaterial.matData.diffuseColor = glm::vec4(material.diffuseColor, 1.0f);
					newMaterial.matData.specularColor = glm::vec4(material.specularColor, 1.0f);
					newMaterial.matData.transparentColor = glm::vec4(material.transparentColor, 1.0f);
					newMaterial.matData.emissiveColor = glm::vec4(material.emissiveColor, 1.0f);

					newMaterial.matData.shininess = material.shininess;
					newMaterial.matData.shininessStr = material.shininessStr;
					newMaterial.matData.opacity = material.opacity;

					newMaterial.matData.hasDiffuseTex = material.hasDiffuseTex;
					newMaterial.matData.hasAmbientTex = material.hasAmbientTex;
					newMaterial.matData.hasSpecularTex = material.hasSpecularTex;
					newMaterial.matData.hasNormalTex = material.hasNormalTex;
					newMaterial.matData.hasShininessTex = material.hasShininessTex;
					newMaterial.matData.hasOpacityTex = material.hasOpacityTex;
					newMaterial.matData.hasEmissionTex = material.hasEmissionTex;

					newMaterial.wireframe = material.wireframe;
					newMaterial.blendMode = material.blendMode;

					if (material.hasDiffuseTex && !loadTexture(material.diffuseTex, newMaterial.diffuseTex))
						return false;
					if (material.hasAmbientTex && !loadTexture(material.ambientTex, newMaterial.ambientTex))
						return false;
					if (material.hasSpecularTex && !loadTexture(material.specularTex, newMaterial.specularTex))
						return false;
					if (material.hasNormalTex && !loadTexture(material.normalTex, newMaterial.normalTex))
						return false;
					if (material.hasShininessTex && !loadTexture(material.shininessTex, newMaterial.shininessTex))
						return false;
					if (material.hasOpacityTex && !loadTexture(material.opacityTex, newMaterial.opacityTex))
						return false;
					if (material.hasEmissionTex && !loadTexture(material.emissionTex, newMaterial.emissionTex))
						return false;

					_scene->materials.push_back(newMaterial);
				}
			}
			return true;
		}

		bool RenderingEngine::loadTexture(const model::Texture2D& in, runtime::Texture2D& out) {
			out.texMapMode = in.texMapMode;

			switch (in.source) {
			case DATA_SOURCE::MEMORY: {
				//================ UNTESTED ======================
				auto data = reinterpret_cast<unsigned char*>(&in.data[0]);
				HRESULT hr = DirectX::CreateDDSTextureFromMemoryEx(_d3dDevice, _deviceContext, data, in.data->size(),
					0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false,
					(ID3D11Resource**)&out.texture, &out.view);
				if (FAILED(hr))
					return false;
				//================================================
			} break;
			case DATA_SOURCE::FILE: {
				std::experimental::filesystem::path path(in.path);
				if (!std::experimental::filesystem::exists(path))
					return false;

				std::string ext = path.extension().string();
				if (ext.compare(".dds") == 0 || ext.compare(".DDS") == 0) {
					HRESULT hr = DirectX::CreateDDSTextureFromFileEx(_d3dDevice, _deviceContext, path.wstring().c_str(),
						0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false,
						(ID3D11Resource**)&out.texture, &out.view);
					if (FAILED(hr))
						return false;
				} else {
					HRESULT hr = DirectX::CreateWICTextureFromFileEx(_d3dDevice, _deviceContext, path.wstring().c_str(),
						0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false,
						(ID3D11Resource**)&out.texture, &out.view);
					if (FAILED(hr))
						return false;
				}
			} break;
			default:
				return false;
			}
			return true;
		}
	}
}
