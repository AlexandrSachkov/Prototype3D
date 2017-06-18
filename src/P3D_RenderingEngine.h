#pragma once

#include <functional>

struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11BlendState;
struct ID3D11DepthStencilView;
struct ID3D11InputLayout;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace p3d
{	
	class RenderingEngine
	{
	public:
		RenderingEngine();
		~RenderingEngine();

		bool initialize(void* windowHandle, int width, int height, bool fullscreen);
		void update(double deltaT);
		void* getDevice();
		void* getDeviceContext();
		void attachUiRenderingCallback(std::function<void(void* device, void* deviceContext)> callback);

	private:
		ID3D11Device* _d3dDevice;
		IDXGISwapChain* _swapChain;
		ID3D11DeviceContext* _deviceContext;
		std::function<void(void* device, void* deviceContext)> _renderUiCallback;

		ID3D11RenderTargetView* _backBufferRenderTargetView;
		ID3D11BlendState* _noBlendState;
		ID3D11DepthStencilView* _depthStencilView;
		ID3D11VertexShader*	_vertexShader;
		ID3D11InputLayout* _inputLayout;
		ID3D11RasterizerState* _backFaceCull;
		ID3D11RasterizerState* _frontFaceCull;
		ID3D11SamplerState*	_textureSamplerState;
		ID3D11PixelShader* _pixelShader;
	};
}
