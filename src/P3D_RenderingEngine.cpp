#include "P3D_RenderingEngine.h"
#include "internal/NL_D3D11Utility.h"

#include <imgui.h>

#include <Windows.h>
#include <d3d11.h>

namespace p3d
{
	RenderingEngine::RenderingEngine() :
		_renderUiCallback([](void* device, void* deviceContext) {})
	{
	}

	RenderingEngine::~RenderingEngine()
	{
	}

	bool RenderingEngine::initialize(void* windowHandle, int width, int height, bool fullscreen)
	{
		D3D11_INPUT_ELEMENT_DESC forwardPosNormTanTextDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		D3D11_INPUT_ELEMENT_DESC forwardPosNormTextDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		D3D11_INPUT_ELEMENT_DESC forwardPosTextDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		D3D11_INPUT_ELEMENT_DESC forwardPosDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

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
		if (!D3D11Utility::createBlendStates(_d3dDevice, false, false, 1, false, _noBlendState))
			return false;
		if (!D3D11Utility::createDepthStencilView(_d3dDevice, width, height, _depthStencilView))
			return false;

		_deviceContext->OMSetRenderTargets(1, &_backBufferRenderTargetView, _depthStencilView);
		//if (!D3D11Utility::loadVertexShader(_d3dDevice, L"Forward_VS.hlsl", _vertexShader))
		//	return false;
		/*if (!D3D11Utility::createInputLayout(_d3dDevice, forwardPosNormTextDesc, ARRAYSIZE(forwardPosNormTextDesc), _vertexShader, _inputLayout))
			return false;
		if (!D3D11Utility::createRasterizerState(_d3dDevice, D3D11_CULL_BACK, D3D11_FILL_SOLID, _backFaceCull))
			return false;
		if (!D3D11Utility::createRasterizerState(_d3dDevice, D3D11_CULL_FRONT, D3D11_FILL_SOLID, _frontFaceCull))
			return false;
		if (!D3D11Utility::createTextureSamplerState(_d3dDevice, _textureSamplerState))
			return false;
		//if (!D3D11Utility::loadPixelShader(_d3dDevice, L"Forward_PS.hlsl", _pixelShader))
		//	return false;

		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };


		_deviceContext->OMSetBlendState(_noBlendState, blendFactor, 0xffffffff);
		//_deviceContext->VSSetShader(_vertexShader.apiVertexShader, nullptr, 0);
		//_deviceContext->PSSetShader(_pixelShader.apiPixelShader, nullptr, 0);
		_deviceContext->IASetInputLayout(_inputLayout);
		D3D11Utility::setViewPort(_deviceContext, (float)screenResolution.width, (float)screenResolution.height);
		_deviceContext->PSSetSamplers(0, 1, &_textureSamplerState);
		_deviceContext->RSSetState(_backFaceCull);

		/*RESOURCES::TransformationBuff transformBuff;
		if (!GRAPHICS::D3D11Utility::createBuffer<RESOURCES::TransformationBuff>(
			_d3dDevice,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_USAGE_DYNAMIC,
			&transformBuff,
			1,
			_transformationBuff
			))
			return false;

		RESOURCES::EyeBuff eyeBuff;
		if (!GRAPHICS::D3D11Utility::createBuffer<RESOURCES::EyeBuff>(
			_d3dDevice,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_USAGE_DYNAMIC,
			&eyeBuff,
			1,
			_eyeBuff
			))
			return false;*/

		return true;
	}

	void* RenderingEngine::getDevice()
	{
		return _d3dDevice;
	}

	void* RenderingEngine::getDeviceContext()
	{
		return _deviceContext;
	}

	void RenderingEngine::attachUiRenderingCallback(std::function<void(void* device, void* deviceContext)> callback)
	{
		_renderUiCallback = callback;
	}

	void RenderingEngine::update(double deltaT)
	{
		float backgroundColor[] = { 0.0f, 0.0f, 0.0f };
		_deviceContext->ClearRenderTargetView(_backBufferRenderTargetView, backgroundColor);

		//draw scene here
		_renderUiCallback(_d3dDevice, _deviceContext);

		_swapChain->Present(0, 0);
	}
}
