#pragma once

#include "internal/p3d_d3d11_RuntimeModel.h"

#include "glm/glm.hpp"
#include "DirectXMath.h"

#include <functional>
#include <memory>
#include <vector>

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
struct ID3D11Buffer;

namespace p3d
{	
	class Camera;
	namespace model {
		struct Scene;
	}
	namespace d3d11
	{
		namespace runtime {
			struct Scene;
			struct Material;
		}
		
		class RenderingEngine
		{
		public:
			RenderingEngine();
			~RenderingEngine();

			bool initialize(void* windowHandle, int width, int height, bool fullscreen);
			void* getDevice();
			void* getDeviceContext();
			void attachUiRenderingCallback(std::function<void(void* device, void* deviceContext)> callback);

			void render(double deltaT, const Camera& camera);
			bool load(const model::Scene& scene);

		private:
			bool RenderingEngine::loadTexture(const model::Texture2D& in, runtime::Texture2D& out);

			ID3D11Device* _d3dDevice;
			IDXGISwapChain* _swapChain;
			ID3D11DeviceContext* _deviceContext;
			std::function<void(void* device, void* deviceContext)> _renderUiCallback;

			ID3D11RenderTargetView* _backBufferRenderTargetView;
			ID3D11DepthStencilView* _depthStencilView;

			ID3D11InputLayout* _untexturedIL;
			ID3D11InputLayout* _texturedIL;
			d3d11::runtime::VertexShader _untexturedVS;
			d3d11::runtime::VertexShader _texturedVS;
			d3d11::runtime::PixelShader _untexturedPS;
			d3d11::runtime::PixelShader _texturedPS;
			
			ID3D11BlendState* _noBlendState;
			ID3D11RasterizerState* _backFaceCull;
			ID3D11RasterizerState* _frontFaceCull;
			ID3D11RasterizerState* _wireframeMode;
			std::array<ID3D11SamplerState*, NUM_TEXTURE_MAP_MODES> _samplerStates;

			ID3D11Buffer* _materialBuff;
			ID3D11Buffer* _transformBuff;

			runtime::Scene* _scene;
			glm::mat4x4 _projection;
		};
	}
}
