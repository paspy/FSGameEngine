#include "precompiled.h"
#include "Renderer.h"
#include "RenderNode.h"
#include "RenderSet.h"

#include "../EDUtilities/InternalOutput.h"
#include "../EDUtilities/Settings.h"

using EDUtilities::InternalOutput;


using std::ifstream;

using namespace DirectX;

namespace EDRendererD3D {
	ID3D11Device * Renderer::theDevicePtr = 0;
	ID3D11DeviceContext * Renderer::theContextPtr = 0;
	IDXGISwapChain * Renderer::theSwapChainPtr = 0;
	ID3D11RenderTargetView * Renderer::theRenderTargetViewPtr = 0;
	ID3D11Texture2D * Renderer::theBackBufferPtr = 0;
	ID3D11Texture2D * Renderer::theDepthStencilBufferPtr = 0;
	ID3D11DepthStencilView * Renderer::theDepthStencilViewPtr = 0;
	ID3D11ShaderResourceView *Renderer::theDepthStencilSRVPtr = 0;

	D3D11_VIEWPORT Renderer::theScreenViewport;
	cbPerObject Renderer::thePerObjectData;
	cbPerSkinnedObject Renderer::thePerSkinnedObjectData;
	ID3D11Buffer *Renderer::thePerObjectCBuffer = 0;
	ID3D11Buffer *Renderer::thePerSkinnedObjectCBuffer = 0;
	UINT Renderer::resolutionWidth = 0;
	UINT Renderer::resolutionHeight = 0;

	UINT Renderer::theRenderCounter = 1;

	bool fullscreen = true;

	void Renderer::Initialize(HWND hWnd, UINT resWidth, UINT resHeight) {
		bool isFullscreen;
		EDUtilities::Settings::GetInstance()->GetSetting("Fullscreen", isFullscreen, false);

		resolutionWidth = resWidth;
		resolutionHeight = resHeight;

		// Begin to create device and swap chains
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL  FeatureLevelsSupported;
		UINT               numLevelsRequested = 1;

		// create a struct to hold information about the swap chain
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		// clear out the struct for use
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// fill the swap chain description struct
		swapChainDesc.BufferDesc.Width = resolutionWidth;
		swapChainDesc.BufferDesc.Height = resolutionHeight;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = isFullscreen;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGIFactory *factoryPtr = nullptr;
		IDXGIAdapter *adapterPtr = nullptr;
		vector<IDXGIAdapter *> adapters;

		CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factoryPtr);

		UINT bestAdapterIndex = 0;
		size_t bestMemSize = 0;
		DXGI_ADAPTER_DESC adapterDesc;
		ZeroMemory(&adapterDesc, sizeof(adapterDesc));

		for ( UINT i = 0; factoryPtr->EnumAdapters(i, &adapterPtr) != DXGI_ERROR_NOT_FOUND; i++ ) {
			adapters.push_back(adapterPtr);
			adapterPtr->GetDesc(&adapterDesc);

			if ( adapterDesc.DedicatedVideoMemory > bestMemSize ) {
				bestAdapterIndex = i;
				bestMemSize = adapterDesc.DedicatedVideoMemory;
			}
		}
		adapters[bestAdapterIndex]->GetDesc(&adapterDesc);
		ReleaseCOM(factoryPtr);

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			adapters[bestAdapterIndex],				// Multiple adapters
			D3D_DRIVER_TYPE_UNKNOWN,				// Driver Type If you specify the adapter, you cannot specify the driver type
			NULL,									// Software
			createDeviceFlags,						// Flags
			&FeatureLevelsRequested,				// Feature Levels Requested Pointer
			1,										// Number of Feature Levels
			D3D11_SDK_VERSION,						// D3D11_SDK_VERSION
			&swapChainDesc,							// Swap Chain Desciptions
			&theSwapChainPtr,							// Swap Chain Pointer
			&theDevicePtr,							// D3D Device
			&FeatureLevelsSupported,				// Return supported levels
			&theContextPtr 					// Device Context Pointer
			);

		BuildPerObjectConstantBuffers();

		//return InitializeSolution(hWnd, isFullscreen);
	}

	void Renderer::ResizeBuffers() {
		// Resize the swap chain and recreate the render target view.
		HR(theSwapChainPtr->ResizeBuffers(1, resolutionWidth, resolutionHeight, DXGI_FORMAT_UNKNOWN, 0));

		ID3D11Texture2D *backBuffer;
		HR(theSwapChainPtr->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
		HR(theDevicePtr->CreateRenderTargetView(backBuffer, 0, &theRenderTargetViewPtr));
		ReleaseCOM(backBuffer);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		// clear out the struct for use
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		theSwapChainPtr->GetDesc(&swapChainDesc);

		// Create the depth/stencil buffer and view.
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = resolutionWidth;
		depthStencilDesc.Height = resolutionHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR(theDevicePtr->CreateTexture2D(&depthStencilDesc, 0, &theDepthStencilBufferPtr));
		HR(theDevicePtr->CreateDepthStencilView(theDepthStencilBufferPtr, &depthStencilViewDesc, &theDepthStencilViewPtr));

		theContextPtr->OMSetRenderTargets(1, &theRenderTargetViewPtr, theDepthStencilViewPtr);

		// Set the viewport transform.
		theScreenViewport.TopLeftX = 0;
		theScreenViewport.TopLeftY = 0;
		theScreenViewport.Width = static_cast<float>(resolutionWidth);
		theScreenViewport.Height = static_cast<float>(resolutionHeight);
		theScreenViewport.MinDepth = 0.0f;
		theScreenViewport.MaxDepth = 1.0f;

		theContextPtr->RSSetViewports(1, &theScreenViewport);

		//return ResizeBuffersSolution();
	}

	void Renderer::SetResolution(UINT _width, UINT _height) {
		// We have to release all references to the swap chain before we can resize it
		ReleaseCOM(theRenderTargetViewPtr);
		ReleaseCOM(theDepthStencilBufferPtr);
		ReleaseCOM(theDepthStencilViewPtr);
		ReleaseCOM(theBackBufferPtr);

		resolutionWidth = _width;
		resolutionHeight = _height;
		ResizeBuffers();
	}

	void Renderer::Shutdown() {
		theSwapChainPtr->SetFullscreenState(false, 0);

		// release the d3d object and device
		ReleaseCOM(theDepthStencilViewPtr);
		ReleaseCOM(theDepthStencilBufferPtr);
		ReleaseCOM(theRenderTargetViewPtr);
		ReleaseCOM(theBackBufferPtr);
		ReleaseCOM(thePerObjectCBuffer);
		ReleaseCOM(thePerSkinnedObjectCBuffer);
		ReleaseCOM(theDepthStencilSRVPtr);

		ReleaseCOM(theSwapChainPtr);
		ReleaseCOM(theContextPtr);
		ReleaseCOM(theDevicePtr);
	}

	void Renderer::Render(RenderSet &set) {
		// Implement a solution for the Renderer Lab
		
		for ( RenderNode *currsor = set.GetHead(); currsor != NULL; currsor = currsor->GetNext() ) {
			currsor->RenderProcess();
		}

		//return RenderSolution(set);
	}

	void Renderer::SetPerObjectData(XMFLOAT4X4 &mMVP, XMFLOAT4X4 &mWorld) {
		thePerObjectData.gMVP = mMVP;
		thePerObjectData.gWorld = mWorld;

		D3D11_MAPPED_SUBRESOURCE edit;
		theContextPtr->Map(thePerObjectCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
		memcpy(edit.pData, &thePerObjectData, sizeof(thePerObjectData));
		theContextPtr->Unmap(thePerObjectCBuffer, 0);

		theContextPtr->VSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
		theContextPtr->PSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
		theContextPtr->GSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
		theContextPtr->DSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
		theContextPtr->HSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
	}

	void Renderer::SetPerObjectData(XMMATRIX &mMVP, XMMATRIX &mWorld) {
		XMFLOAT4X4 tempMVP;
		XMFLOAT4X4 tempWorld;

		XMStoreFloat4x4(&tempMVP, mMVP);
		XMStoreFloat4x4(&tempWorld, mWorld);

		SetPerObjectData(tempMVP, tempWorld);
	}

	void Renderer::SetPerObjectData(DirectX::XMFLOAT4X4 &mViewProj,
		XMFLOAT4X4 &mWorld, XMFLOAT4X4 SkinnedMatrices[cbPerSkinnedObject::NUM_BONES]) {
		thePerSkinnedObjectData.gSkinnedViewProj = mViewProj;
		//thePerSkinnedObjectData.gSkinnedWorld = mWorld;

		memcpy(thePerSkinnedObjectData.gSkinnedMatrices, SkinnedMatrices,
			cbPerSkinnedObject::NUM_BONES * sizeof(XMFLOAT4X4));

		D3D11_MAPPED_SUBRESOURCE edit;
		theContextPtr->Map(thePerSkinnedObjectCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
		memcpy(edit.pData, &thePerSkinnedObjectData, sizeof(thePerSkinnedObjectData));
		theContextPtr->Unmap(thePerSkinnedObjectCBuffer, 0);

		theContextPtr->VSSetConstantBuffers(cbPerSkinnedObject::REGISTER_SLOT, 1,
			&thePerSkinnedObjectCBuffer);
		theContextPtr->PSSetConstantBuffers(cbPerSkinnedObject::REGISTER_SLOT, 1,
			&thePerSkinnedObjectCBuffer);
		theContextPtr->GSSetConstantBuffers(cbPerSkinnedObject::REGISTER_SLOT, 1,
			&thePerSkinnedObjectCBuffer);
		theContextPtr->DSSetConstantBuffers(cbPerSkinnedObject::REGISTER_SLOT, 1,
			&thePerSkinnedObjectCBuffer);
		theContextPtr->HSSetConstantBuffers(cbPerSkinnedObject::REGISTER_SLOT, 1,
			&thePerSkinnedObjectCBuffer);

	}

	ID3D11ShaderResourceView *Renderer::GetDepthSRV() {
		// Do not try to create a view to an uninitialized resource
		assert(theDepthStencilBufferPtr);
		if ( !theDepthStencilSRVPtr ) {
			// Create the requested view
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			//desc.Buffer = 0; //?
			desc.Texture2D.MipLevels = -1;
			desc.Texture2D.MostDetailedMip = 0;
			HRESULT hr = Renderer::theDevicePtr->CreateShaderResourceView(
				theDepthStencilBufferPtr, &desc, &theDepthStencilSRVPtr);
			HR(hr);
			SetD3DName(theDepthStencilSRVPtr, "Depth Stencil SRV");
		}
		return theDepthStencilSRVPtr;
	}

	void Renderer::BuildPerObjectConstantBuffers() {
		// per object CBuffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(cbPerObject);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		HR(theDevicePtr->CreateBuffer(&bd, nullptr, &thePerObjectCBuffer));

		// per object CBuffer
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(cbPerSkinnedObject);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		HR(theDevicePtr->CreateBuffer(&bd, nullptr, &thePerSkinnedObjectCBuffer));
	}

	void Renderer::Render(RenderSet &set, EDRenderFunc renderFuncOverride) {
		RenderNode *pCurrent = set.GetHead();

		while ( 0 != pCurrent ) {
			renderFuncOverride(*pCurrent);
			pCurrent = pCurrent->GetNext();
		}
	}
}