#pragma once

#include "../p3d_ResourceModel.h"
#include "p3d_d3d11_RuntimeModel.h"

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "glm/glm.hpp"

#include <locale>
#include <codecvt>
#include <string>

namespace p3d
{
	namespace d3d11
	{
		namespace util
		{
			bool createConstBuffDynamic(ID3D11Device* device, void* data, unsigned int size, ID3D11Buffer*& out)
			{
				D3D11_BUFFER_DESC cbDesc;
				cbDesc.ByteWidth = size;
				cbDesc.Usage = D3D11_USAGE_DYNAMIC;
				cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				cbDesc.MiscFlags = 0;
				cbDesc.StructureByteStride = 0;

				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = data;
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;

				HRESULT hr = device->CreateBuffer(&cbDesc, &InitData, &out);

				if (FAILED(hr))
					//display error message
					return false;

				return true;
			}

			bool createConstBuffStatic(ID3D11Device* device, void* data, unsigned int size, ID3D11Buffer*& out)
			{
				D3D11_BUFFER_DESC cbDesc;
				cbDesc.ByteWidth = size;
				cbDesc.Usage = D3D11_USAGE_DEFAULT;
				cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				cbDesc.CPUAccessFlags = 0;
				cbDesc.MiscFlags = 0;
				cbDesc.StructureByteStride = 0;

				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = data;
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;

				HRESULT hr = device->CreateBuffer(&cbDesc, &InitData, &out);

				if (FAILED(hr))
					//display error message
					return false;

				return true;
			}

			void updateConstBuff(
				ID3D11DeviceContext* deviceContext,
				ID3D11Buffer* buffer,
				void* data,
				size_t dataSize
				)
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

				deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, data, dataSize);
				deviceContext->Unmap(buffer, 0);
			}

			D3D11_PRIMITIVE_TOPOLOGY convertTopology(PRIMITIVE_TOPOLOGY_TYPE topology)
			{
				switch (topology)
				{
				case LINE_LIST:
					return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				case TRIANGLE_LIST:
					return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

				default:
					return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
				}
			}

			D3D11_TEXTURE_ADDRESS_MODE convertTextMapMode(TEXTURE_MAP_MODE mode)
			{
				switch (mode)
				{
				case WRAP:
					return D3D11_TEXTURE_ADDRESS_WRAP;
				case MIRROR:
					return D3D11_TEXTURE_ADDRESS_MIRROR;
				case CLAMP:
					return D3D11_TEXTURE_ADDRESS_CLAMP;

				default:
					return D3D11_TEXTURE_ADDRESS_WRAP;
				}
			}

			bool createVertexBuffStatic(ID3D11Device* device, void* data, unsigned int size, unsigned int num, ID3D11Buffer*& out)
			{
				D3D11_BUFFER_DESC bufferDesc = {};
				bufferDesc.Usage = D3D11_USAGE_DEFAULT;
				bufferDesc.ByteWidth = size * num;
				bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bufferDesc.CPUAccessFlags = 0;
				bufferDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = data;
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;

				HRESULT hr = device->CreateBuffer(&bufferDesc, &InitData, &out);
				if (FAILED(hr))
					//display error message
					return false;

				return true;
			}

			bool createIndexBuffStatic(ID3D11Device* device, void* data, unsigned int num, ID3D11Buffer*& out)
			{
				D3D11_BUFFER_DESC bufferDesc = {};
				bufferDesc.Usage = D3D11_USAGE_DEFAULT;
				bufferDesc.ByteWidth = sizeof(unsigned int) * num;
				bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bufferDesc.CPUAccessFlags = 0;
				bufferDesc.MiscFlags = 0;

				// Define the resource data.
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = data;
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;

				// Create the buffer with the device.
				HRESULT hr = device->CreateBuffer(&bufferDesc, &InitData, &out);
				if (FAILED(hr))
					return false;

				return true;
			}

			bool loadBlobFromFile(std::string path, ID3DBlob*& blob)
			{
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				std::wstring wpath = converter.from_bytes(path);
				HRESULT hr = D3DReadFileToBlob(wpath.c_str(), &blob);
				if (FAILED(hr))
				{
					//CONSOLE::out(CONSOLE::ERR, L"Failed to load blob.\n");
					return false;
				}
				return true;
			}

			bool compileBlobFromFile(std::string path, LPCSTR entryPoint, LPCSTR profile, ID3DBlob*& blob)
			{
				UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#if defined( DEBUG ) || defined( _DEBUG )
				flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				std::wstring wpath = converter.from_bytes(path);
				ID3DBlob* errorBlob = nullptr;
				HRESULT hr = D3DCompileFromFile(wpath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entryPoint, profile,
					flags, 0, &blob, &errorBlob);
				if (FAILED(hr))
				{
					if (errorBlob)
					{
						OutputDebugStringA((char*)errorBlob->GetBufferPointer());
						errorBlob->Release();

						//CONSOLE::out(CONSOLE::ERR, L"Failed to compile blob.\n");
					}
					return false;
				}
				return true;
			}


			bool loadVertexShader(ID3D11Device* device, std::string path, runtime::VertexShader& vertexShader)
			{
				//Load vertex shader from file
				std::string source(".hlsl");
				std::string binary(".cso");

				bool result;
				if (path.rfind(source) != std::string::npos)
				{
					result = compileBlobFromFile(path, "main", "vs_5_0", vertexShader.blob);
				}
				else if (path.rfind(binary) != std::string::npos)
				{
					result = loadBlobFromFile(path, vertexShader.blob);
				}
				else
				{
					//CONSOLE::out(CONSOLE::ERR, L"Invalid shader file extension: " + path);
					return false;
				}

				if (!result)
				{
					//CONSOLE::out(CONSOLE::ERR, L"Failed to load Vertex Shader: " + path);
					return false;
				}

				//Create Vertex Shader
				HRESULT hr = device->CreateVertexShader(vertexShader.blob->GetBufferPointer(), vertexShader.blob->GetBufferSize(), NULL, &vertexShader.shader);
				if (FAILED(hr))
				{
					//CONSOLE::out(CONSOLE::ERR, L"Failed to create Vertex Shader");
					return false;
				}

				return true;
			}

			bool loadPixelShader(ID3D11Device* device, std::string path, runtime::PixelShader& pixelShader)
			{
				//Load pixel shader from file
				std::string source(".hlsl");
				std::string binary(".cso");

				bool result;
				if (path.rfind(source) != std::string::npos)
				{
					result = compileBlobFromFile(path, "main", "ps_5_0", pixelShader.blob);
				}
				else if (path.rfind(binary) != std::string::npos)
				{
					result = loadBlobFromFile(path, pixelShader.blob);
				}
				else
				{
					//CONSOLE::out(CONSOLE::ERR, L"Invalid shader file extension: " + path);
					return false;
				}

				if (!result)
				{
					//CONSOLE::out(CONSOLE::ERR, L"Failed to load Pixel Shader: " + path);
					return false;
				}

				//Create Pixel Shader
				ID3D11PixelShader* ps = NULL;
				HRESULT hr = device->CreatePixelShader(pixelShader.blob->GetBufferPointer(), pixelShader.blob->GetBufferSize(), NULL, &pixelShader.shader);
				if (FAILED(hr))
				{
					//CONSOLE::out(CONSOLE::ERR, L"Failed to create Pixel Shader");
					return false;
				}

				return true;
			}

			bool createInputLayout(
				ID3D11Device* device,
				D3D11_INPUT_ELEMENT_DESC ilDesc[],
				uint_fast32_t size,
				runtime::VertexShader& vShader,
				ID3D11InputLayout*& inputLayout
				)
			{
				HRESULT hr;
				hr = device->CreateInputLayout(ilDesc, size, vShader.blob->GetBufferPointer(),
					vShader.blob->GetBufferSize(), &inputLayout);
				if (FAILED(hr))
				{
					//CONSOLE::out(CONSOLE::ERR, L"Failed to create Input Layout");
					return false;
				}
				return true;
			}

			void setViewPort(
				ID3D11DeviceContext* deviceContext,
				uint_fast32_t numViewports,
				glm::vec2 topLeft,
				glm::vec2 dimensions,
				glm::vec2 minMaxDepth
				)
			{
				D3D11_VIEWPORT viewport;
				ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

				viewport.TopLeftX = topLeft.x;
				viewport.TopLeftY = topLeft.y;
				viewport.Width = dimensions.x;
				viewport.Height = dimensions.y;
				viewport.MinDepth = minMaxDepth.x;
				viewport.MaxDepth = minMaxDepth.y;

				//Set the Viewport
				deviceContext->RSSetViewports(numViewports, &viewport);
			}

			bool createTextureSamplerState(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE mode, ID3D11SamplerState*& samplerState) {
				D3D11_SAMPLER_DESC sampDesc;
				ZeroMemory(&sampDesc, sizeof(sampDesc));
				sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				sampDesc.AddressU = mode;
				sampDesc.AddressV = mode;
				sampDesc.AddressW = mode;
				sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				sampDesc.MinLOD = 0;
				sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

				//Create the Sample State
				HRESULT hr = device->CreateSamplerState(&sampDesc, &samplerState);
				if (FAILED(hr)) {
					//CONSOLE::out(CONSOLE::ERR, L"Failed to create texture sampler state");
					return false;
				}
				return true;
			}
		}
	}
}