#include "d3d11_Utility.h"

#include <d3dcompiler.h>
#include <assert.h>

namespace p3d {
	namespace d3d11 {

		bool Utility::createDevice(
			ComPtr<ID3D11Device>& device,
			ComPtr<ID3D11DeviceContext>& deviceContext
		) {
			unsigned int creationFlags = 0;
		#if defined(P3D_DEBUG)
			creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

			D3D_FEATURE_LEVEL featureLevel;
			P3D_ASSERT_R_DX11(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				creationFlags,
				nullptr, 
				0,
				D3D11_SDK_VERSION,
				&device, &featureLevel, &deviceContext));

			P3D_ASSERT_R(featureLevel == D3D_FEATURE_LEVEL_11_0, "Direct3D Feature Level 11 unsupported");

			return true;
		}

		bool Utility::getMSAAQualityLevel(
			ComPtr<ID3D11Device> device,
			unsigned int msaaLvl,
			unsigned int& qualityLvl
		) {
			P3D_ASSERT_R_DX11(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, msaaLvl, &qualityLvl));
			P3D_ASSERT_R(qualityLvl > 0, "MSAA " << msaaLvl << " is not supported");

			return true;
		}

		bool Utility::setFullScreen(
			ComPtr<IDXGISwapChain> swapChain,
			bool fullscreen
		) {
			P3D_ASSERT_R_DX11(swapChain->SetFullscreenState(fullscreen, nullptr));
			return true;
		}

		bool Utility::createSwapChain(
			HWND windowHandle,
			unsigned int screenRefreshRate,
			Vec2_uint screenDim,
			unsigned int msaaLevel,
			unsigned int msaaQualityLevel,
			unsigned int numBackBuffers,
			bool fullScreen,
			ComPtr<ID3D11Device> device,
			ComPtr<IDXGISwapChain>& swapChain
		) {
			DXGI_MODE_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

			bufferDesc.Width = screenDim.x;
			bufferDesc.Height = screenDim.y;
			bufferDesc.RefreshRate.Numerator = screenRefreshRate;
			bufferDesc.RefreshRate.Denominator = 1;
			bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			DXGI_SWAP_CHAIN_DESC swapChainDesc;
			ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

			swapChainDesc.BufferDesc = bufferDesc;
			swapChainDesc.SampleDesc.Count = msaaLevel;
			swapChainDesc.SampleDesc.Quality = msaaQualityLevel;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = numBackBuffers;
			swapChainDesc.OutputWindow = windowHandle;
			swapChainDesc.Windowed = true;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;

			ComPtr<IDXGIDevice> dxgiDevice;
			P3D_ASSERT_R_DX11(device->QueryInterface(__uuidof(IDXGIDevice),(void**)&dxgiDevice));

			ComPtr<IDXGIAdapter> dxgiAdapter;
			P3D_ASSERT_R_DX11(dxgiDevice->GetParent(__uuidof(IDXGIAdapter),(void**)&dxgiAdapter));

			ComPtr<IDXGIFactory> dxgiFactory;
			P3D_ASSERT_R_DX11(dxgiAdapter->GetParent(__uuidof(IDXGIFactory),(void**)&dxgiFactory));

			P3D_ASSERT_R_DX11(dxgiFactory->CreateSwapChain(device.Get(), &swapChainDesc, &swapChain));

			//Microsoft suggests not creating a full-screen swap chain
			//"This can reduce presentation performance if the swap chain size and the output window size do not match"
			//https://docs.microsoft.com/en-gb/windows/desktop/api/dxgi/nf-dxgi-idxgifactory-createswapchain
			if (fullScreen) { 
				P3D_ASSERT_R(setFullScreen(swapChain, true), "Unable to set swapchain to full screen");
			}

			return true;
		}

		bool Utility::createBackBufferRenderTargetView(
			ComPtr<ID3D11Device> device,
			ComPtr<IDXGISwapChain> swapChain,
			ComPtr<ID3D11Texture2D>& renderTargetBuff,
			ComPtr<ID3D11RenderTargetView>& renderTargetView
		) {
			ComPtr<ID3D11Texture2D> rtBuff;
			P3D_ASSERT_R_DX11(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &rtBuff));
			P3D_ASSERT_R_DX11(device->CreateRenderTargetView(rtBuff.Get(), nullptr, &renderTargetView));
			renderTargetBuff = rtBuff;
			return true;
		}

		bool Utility::createDepthStencilView(
			ComPtr<ID3D11Device> device,
			Vec2_uint screenDim,
			unsigned int msaaLevel,
			unsigned int msaaQualityLevel,
			ComPtr<ID3D11Texture2D>& depthStencilBuff,
			ComPtr<ID3D11DepthStencilView>& depthStencilView
		) {
			D3D11_TEXTURE2D_DESC depthStencilDesc;

			depthStencilDesc.Width = screenDim.x;
			depthStencilDesc.Height = screenDim.y;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDesc.SampleDesc.Count = msaaLevel;
			depthStencilDesc.SampleDesc.Quality = msaaQualityLevel;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.CPUAccessFlags = 0;
			depthStencilDesc.MiscFlags = 0;

			P3D_ASSERT_R_DX11(device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuff));
			P3D_ASSERT_R_DX11(device->CreateDepthStencilView(depthStencilBuff.Get(), nullptr, &depthStencilView));

			return true;
		}

		bool Utility::compileShader(
			std::string source,
			std::string entryPoint,
			std::string target,
			ComPtr<ID3DBlob>& blob,
			ComPtr<ID3DBlob>& errBlob
		) {
			UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
		#if defined( P3D_DEBUG )
			flags |= D3DCOMPILE_DEBUG;
		#endif

			P3D_ASSERT_R_DX11(D3DCompile(
				source.data(),
				source.length(),
				nullptr,
				nullptr,
				nullptr,
				entryPoint.data(),
				target.c_str(),
				flags,
				0,
				&blob,
				&errBlob
			));

			return true;
		}

		bool Utility::createVertexShader(
			ComPtr<ID3D11Device> device,
			ComPtr<ID3DBlob> blob,
			ComPtr<ID3D11VertexShader>& vs
		) {
			P3D_ASSERT_R_DX11(device->CreateVertexShader(
				blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vs
			));
			return true;
		}

		bool Utility::createPixelShader(
			ComPtr<ID3D11Device> device,
			ComPtr<ID3DBlob> blob,
			ComPtr<ID3D11PixelShader>& ps
		) {
			P3D_ASSERT_R_DX11(device->CreatePixelShader(
				blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ps
			));
			return true;
		}

		bool Utility::createInputLayout(
			ComPtr<ID3D11Device> device,
			ComPtr<ID3DBlob> vsShaderBlob,
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& elementDesc,
			ComPtr<ID3D11InputLayout>& inputLayout
		) {
			P3D_ASSERT_R_DX11(device->CreateInputLayout(
				elementDesc.data(), 
				(UINT)elementDesc.size(), 
				vsShaderBlob->GetBufferPointer(), 
				vsShaderBlob->GetBufferSize(), 
				&inputLayout
			));
			return true;
		}

		bool Utility::createBlendStates(
			ID3D11Device* device,
			bool enableBlend,
			bool enableIndependentBlending,
			uint_fast32_t numRenderTargets,
			bool enableAlphaToCoverage,
			ID3D11BlendState*& blendState
		) {
			assert(numRenderTargets > 0 && numRenderTargets <= 8);

			HRESULT hr;
			D3D11_BLEND_DESC blendDesc;
			ZeroMemory(&blendDesc, sizeof(blendDesc));

			D3D11_RENDER_TARGET_BLEND_DESC rtbd;
			ZeroMemory(&rtbd, sizeof(rtbd));

			rtbd.BlendEnable = enableBlend;
			rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
			rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
			rtbd.BlendOp = D3D11_BLEND_OP_ADD;
			rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
			rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
			rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

			blendDesc.IndependentBlendEnable = enableIndependentBlending;
			blendDesc.AlphaToCoverageEnable = enableAlphaToCoverage;

			if (enableIndependentBlending) {
				for (uint_fast32_t i = 0; i < numRenderTargets; i++) {
					blendDesc.RenderTarget[i] = rtbd;
				}
			} else {
				blendDesc.RenderTarget[0] = rtbd;
			}


			hr = device->CreateBlendState(&blendDesc, &blendState);
			if (FAILED(hr)) {
				//CONSOLE::out(CONSOLE::ERR, L"Failed to create blend state");
				return false;
			}
			return true;
		}

		bool Utility::createShaderResourceViewFromTexture2D(
			ID3D11Device* device,
			ID3D11Texture2D* texture,
			ID3D11ShaderResourceView*& resourceView
		) {
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //changes based on the image format
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = -1;
			srvDesc.Texture2D.MostDetailedMip = 0;

			HRESULT hr;
			hr = device->CreateShaderResourceView(texture, &srvDesc, &resourceView);
			if (FAILED(hr)) {
				//CONSOLE::out(CONSOLE::ERR, L"Failed to create Shader Resource View from Texture2D");
				return false;
			}
			return true;
		}

		bool Utility::createTexture2D(
			ID3D11Device* device,
			uint_fast32_t width,
			uint_fast32_t height,
			bool generateMipMaps,
			uint_fast32_t MSAALevel,
			D3D11_USAGE usage,
			D3D11_BIND_FLAG bindFlag,
			void* data,
			uint_fast32_t memPitch,
			ID3D11Texture2D*& texture
		) {
			D3D11_TEXTURE2D_DESC textDesc;
			ZeroMemory(&textDesc, sizeof(textDesc));
			textDesc.Width = width;
			textDesc.Height = height;
			textDesc.MipLevels = generateMipMaps == true ? 0 : 1;
			textDesc.ArraySize = 1;
			textDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //needs to change based on texture format

			DXGI_SAMPLE_DESC sampleDesc;
			ZeroMemory(&sampleDesc, sizeof(sampleDesc));
			sampleDesc.Count = MSAALevel;
			sampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;

			textDesc.SampleDesc = sampleDesc;
			textDesc.Usage = usage;
			textDesc.BindFlags = bindFlag;
			textDesc.CPUAccessFlags = usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
			textDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA subresData;
			ZeroMemory(&subresData, sizeof(subresData));
			subresData.pSysMem = data;
			subresData.SysMemPitch = memPitch;

			HRESULT hr;
			hr = device->CreateTexture2D(&textDesc, &subresData, &texture);
			if (FAILED(hr)) {
				//CONSOLE::out(CONSOLE::ERR, L"Failed to create texture");
				return false;
			}
			return true;
		}

		bool Utility::createTextureSamplerState(ID3D11Device* device, ID3D11SamplerState*& samplerState) {
			HRESULT hr;

			D3D11_SAMPLER_DESC sampDesc;
			ZeroMemory(&sampDesc, sizeof(sampDesc));
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

			//Create the Sample State
			hr = device->CreateSamplerState(&sampDesc, &samplerState);
			if (FAILED(hr)) {
				//CONSOLE::out(CONSOLE::ERR, L"Failed to create texture sampler state");
				return false;
			}

			return true;
		}



		bool Utility::createRasterizerState(
			ID3D11Device* device,
			D3D11_CULL_MODE cullMode,
			D3D11_FILL_MODE fillMode,
			ID3D11RasterizerState*& rasterizerState
		) {
			HRESULT hr;

			D3D11_RASTERIZER_DESC cmdesc;
			ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

			cmdesc.CullMode = cullMode;
			cmdesc.FillMode = fillMode;
			cmdesc.FrontCounterClockwise = true;

			hr = device->CreateRasterizerState(&cmdesc, &rasterizerState);
			if (FAILED(hr)) {
				//CONSOLE::out(CONSOLE::ERR, L"Failed to create rasterizer state");
				return false;
			}

			return true;
		}

		bool Utility::compileBlobFromFile(std::wstring path, LPCSTR entryPoint, LPCSTR profile, ID3DBlob*& blob) {
			UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
		#if defined( DEBUG ) || defined( _DEBUG )
			flags |= D3DCOMPILE_DEBUG;
		#endif

			ID3DBlob* errorBlob = nullptr;
			HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				entryPoint, profile,
				flags, 0, &blob, &errorBlob);
			if (FAILED(hr)) {
				if (errorBlob) {
					OutputDebugStringA((char*)errorBlob->GetBufferPointer());
					errorBlob->Release();

					//CONSOLE::out(CONSOLE::ERR, L"Failed to compile blob.\n");
				}
				return false;
			}
			return true;
		}

		bool Utility::loadBlobFromFile(std::wstring path, ID3DBlob*& blob) {
			HRESULT hr;

			hr = D3DReadFileToBlob(path.c_str(), &blob);
			if (FAILED(hr)) {
				//CONSOLE::out(CONSOLE::ERR, L"Failed to load blob.\n");
				return false;
			}
			return true;
		}


		/*bool D3D11Utility::loadVertexShader(ID3D11Device* device, std::wstring path, RESOURCES::VertexShader& vertexShader)
		{
			HRESULT hr;
			//Load vertex shader from file
			std::wstring source(L".hlsl");
			std::wstring binary(L".cso");

			bool result;
			if (path.rfind(source) != std::string::npos)
			{
				result = compileBlobFromFile(path, "VSMain", "vs_5_0", vertexShader.blob);
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
			hr = device->CreateVertexShader(vertexShader.blob->GetBufferPointer(), vertexShader.blob->GetBufferSize(), NULL, &vertexShader.apiVertexShader);
			if (FAILED(hr))
			{
				//CONSOLE::out(CONSOLE::ERR, L"Failed to create Vertex Shader");
				return false;
			}

			return true;
		}*/

		/*bool D3D11Utility::loadPixelShader(ID3D11Device* device, std::wstring path, RESOURCES::PixelShader& pixelShader)
		{
			HRESULT hr;

			//Load pixel shader from file
			std::wstring source(L".hlsl");
			std::wstring binary(L".cso");

			bool result;
			if (path.rfind(source) != std::string::npos)
			{
				result = compileBlobFromFile(path, "PSMain", "ps_5_0", pixelShader.blob);
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
			hr = device->CreatePixelShader(pixelShader.blob->GetBufferPointer(), pixelShader.blob->GetBufferSize(), NULL, &pixelShader.apiPixelShader);
			if (FAILED(hr))
			{
				//CONSOLE::out(CONSOLE::ERR, L"Failed to create Pixel Shader");
				return false;
			}

			return true;
		}*/
	}
}


