#include "d3d11_RenderingDevice.h"
#include "../../assert.h"
#include "../dx/dx_ConstConvert.h"
#include "d3d11_Utility.h"

#include "d3d11_Texture1dArray.h"
#include "d3d11_Texture2dArray.h"
#include "d3d11_Texture3d.h"
#include "d3d11_VertexShader.h"
#include "d3d11_PixelShader.h"

#include <Windows.h>

#include <algorithm>

namespace p3d {
	namespace d3d11 {
		RenderingDevice::RenderingDevice() {
			
		}

		RenderingDevice::~RenderingDevice() {
			release();
		}

		bool RenderingDevice::initialize(
			HWND windowHandle,
			unsigned int screenRefreshRate,
			p3d::Vec2_uint screenDim,
			unsigned int msaaLevel,
			unsigned int numBackBuffers,
			bool fullscreen
		) {
			P3D_ASSERT_R(Utility::createDevice(_device, _deviceContext), "d3d11 device failed to initialize");

			if (msaaLevel < 1) msaaLevel = 1; //anything less than 1 is considered off. So we set the sampling count to 1

			unsigned int maxMsaaQualityLvl = 0;
			if (msaaLevel > 1) {
				P3D_ASSERT_R(Utility::getMSAAQualityLevel(_device, msaaLevel, maxMsaaQualityLvl), "Failed to retrieve MSAA quality level");
			}

			unsigned int msaaQualityLevel = maxMsaaQualityLvl - 1;
			
			P3D_ASSERT_R(Utility::createSwapChain(
				windowHandle,
				screenRefreshRate,
				screenDim,
				msaaLevel,
				msaaQualityLevel,
				numBackBuffers,
				fullscreen,
				_device,
				_swapChain
			), "Failed to create swap chain");

			P3D_ASSERT_R(Utility::createBackBufferRenderTargetView(
				_device,
				_swapChain,
				_backBuffRenderTarget,
				_backBuffRenderTargetView
			), "Failed to create back buffer render target view");

			P3D_ASSERT_R(Utility::createDepthStencilView(
				_device,
				screenDim,
				msaaLevel,
				msaaQualityLevel,
				_depthStencilBuff,
				_depthStencilView
			), "Failed to create depth stencil view");

			return true;
		}

		void RenderingDevice::release() {
			if (_swapChain) Utility::setFullScreen(_swapChain, false);

			_depthStencilBuff = nullptr;
			_depthStencilView = nullptr;
			_backBuffRenderTargetView = nullptr;
			_swapChain		= nullptr;
			_deviceContext	= nullptr;
			_device			= nullptr;
		}

		bool RenderingDevice::createTexture1dArray(const Texture1dArrayDesc& desc, p3d::Texture1dArrayI*& tex) {
			//TODO: create GPU texture
			tex = new Texture1dArray({ nullptr }, desc);
			return true;
		}

		bool RenderingDevice::createTexture2dArray(const Texture2dArrayDesc& desc, p3d::Texture2dArrayI*& tex) {
			//TODO: create GPU texture array
			tex = new Texture2dArray({ nullptr }, desc);
			return true;
		}

		bool RenderingDevice::createTexture3d(const Texture3dDesc& desc, p3d::Texture3dI*& tex) {
			//TODO: create GPU texture array
			tex = new Texture3d({ nullptr }, desc);
			return true;
		}

		bool RenderingDevice::createVertexShader(const VertexShaderDesc& desc, p3d::VertexShaderI*& vs) {
			ComPtr<ID3DBlob> blob = nullptr;
			ComPtr<ID3DBlob> errBlob = nullptr;
			P3D_ASSERT_R(Utility::compileShader(desc.hlslSource, desc.shaderEntryPoint, "vs_5_0", blob, errBlob), 
				"Failed to compile vertex shader: " + std::string((char*)errBlob->GetBufferPointer())
			);
			
			ComPtr<ID3D11VertexShader> shader = nullptr;
			P3D_ASSERT_R(Utility::createVertexShader(_device, blob, shader), "Failed to create vertex shader");

			ComPtr<ID3D11InputLayout> inputLayout = nullptr;
			P3D_ASSERT_R(createInputLayout(desc.inputDesc, blob, inputLayout), "Failed to create input layout");

			vs = new VertexShader(shader, blob, inputLayout, desc);
			return true;
		}

		bool RenderingDevice::createPixelShader(const PixelShaderDesc& desc, p3d::PixelShaderI*& ps) {
			ComPtr<ID3DBlob> blob = nullptr;
			ComPtr<ID3DBlob> errBlob = nullptr;
			P3D_ASSERT_R(Utility::compileShader(desc.hlslSource, desc.shaderEntryPoint, "ps_5_0", blob, errBlob),
				"Failed to compile pixel shader: " + std::string((char*)errBlob->GetBufferPointer())
			);

			ComPtr<ID3D11PixelShader> shader = nullptr;
			P3D_ASSERT_R(Utility::createPixelShader(_device, blob, shader), "Failed to create pixel shader");

			ps = new PixelShader(shader, blob, desc);
			return true;
		}

		bool RenderingDevice::createInputLayout(
			const std::vector<VertexShaderDesc::InputElementDesc>& inputDesc,
			ComPtr<ID3DBlob> vsBlob,
			ComPtr<ID3D11InputLayout>& inputLayout
		) {
			std::vector<VertexShaderDesc::InputElementDesc> p3dInputDescTemp(
				inputDesc.begin(), inputDesc.end()
			);

			//sort by input slot in assending order
			std::sort(p3dInputDescTemp.begin(), p3dInputDescTemp.end(),
				[](VertexShaderDesc::InputElementDesc i, VertexShaderDesc::InputElementDesc j) {
				return i.inputSlot < j.inputSlot;
			}
			);

			unsigned int currInputSlot = 0;
			unsigned int currOffset = 0;
			std::vector<D3D11_INPUT_ELEMENT_DESC> elementDesc;
			for (int i = 0; i < p3dInputDescTemp.size(); i++) {// auto p3dElemDesc : p3dInputDescTemp) {
				auto p3dElemDesc = p3dInputDescTemp[i];
				if (currInputSlot != p3dElemDesc.inputSlot) {
					currInputSlot = p3dElemDesc.inputSlot;
					currOffset = 0;
				}
				elementDesc.push_back({
					p3dInputDescTemp[i].elementName.c_str(),
					0,
					dx::convertFormat(p3dElemDesc.dataFormat),
					currInputSlot,
					currOffset,
					D3D11_INPUT_PER_VERTEX_DATA,
					0
					});
				currOffset += p3dElemDesc.dataSizeBytes;
			}

			return Utility::createInputLayout(_device, vsBlob, elementDesc, inputLayout);
		}
	}
}