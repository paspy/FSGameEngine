#include "precompiled.h"
#include "RenderTarget.h"
#include "Renderer.h"

#include "../EDMemoryManager/MemoryManager.h"
using namespace EDMemoryManager;



namespace EDRendererD3D {
	RenderTargetSurface::RenderTargetSurface() {
		textureTargetPtr = 0;
	}

	RenderTargetSurface::~RenderTargetSurface() {
		Destroy();
	}

	void RenderTargetSurface::Destroy() {
		if ( textureTargetPtr ) {
			delete textureTargetPtr;
			textureTargetPtr = 0;
		}
	}

	void RenderTargetSurface::Create(UINT _width, UINT _height, DXGI_FORMAT format, const char * name) {
		width = _width;
		height = _height;
		// Implement a solution for the Deferred Renderer Lab

		D3D11_TEXTURE2D_DESC RTSdesc;
		ZeroMemory(&RTSdesc, sizeof(D3D11_TEXTURE2D_DESC));

		RTSdesc.Height = _height;
		RTSdesc.Width = _width;
		RTSdesc.Format = format;
		RTSdesc.Usage = D3D11_USAGE_DEFAULT;
		RTSdesc.ArraySize = 1;
		RTSdesc.MipLevels = 1;
		RTSdesc.SampleDesc.Count = 1;
		RTSdesc.SampleDesc.Quality = 0;
		RTSdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		RTSdesc.CPUAccessFlags = 0;
		RTSdesc.MiscFlags = 0;
		
		textureTargetPtr = new RenderTexture2D;

		textureTargetPtr->Create(RTSdesc, name);

		//return CreateSolution(_width, _height, format, name);
	}

	RenderTarget::RenderTarget() {
		depthStecilView = 0;
		depthTexturePtr = 0;

		bShouldDeleteDepth = true;
	}

	RenderTarget::~RenderTarget() {
		Destroy();
	}

	void RenderTarget::Create(UINT _width, UINT _height, DXGI_FORMAT format) {
		Destroy();
		bShouldDeleteDepth = true;

		// Implement a solution for the Deferred Renderer Lab

		D3D11_TEXTURE2D_DESC RTdesc;
		ZeroMemory(&RTdesc, sizeof(D3D11_TEXTURE2D_DESC));

		RTdesc.Height = _height;
		RTdesc.Width = _width;
		RTdesc.Format = format;
		RTdesc.Usage = D3D11_USAGE_DEFAULT;
		RTdesc.ArraySize = 1;
		RTdesc.MipLevels = 1;
		RTdesc.SampleDesc.Count = 1;
		RTdesc.SampleDesc.Quality = 0;
		RTdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		RTdesc.CPUAccessFlags = 0;
		RTdesc.MiscFlags = 0;

		Renderer::theDevicePtr->CreateTexture2D(&RTdesc, 0, (ID3D11Texture2D **)&depthTexturePtr);

		D3D11_DEPTH_STENCIL_VIEW_DESC DSVdesc;
		ZeroMemory(&DSVdesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		DSVdesc.Format = format;
		DSVdesc.Flags = 0;
		DSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		DSVdesc.Texture2D.MipSlice = 0;

		Renderer::theDevicePtr->CreateDepthStencilView(depthTexturePtr, &DSVdesc, (ID3D11DepthStencilView **)&depthStecilView);

		//return CreateSolution(_width, _height, format);
	}

	void RenderTarget::Create(ID3D11DepthStencilView * _depthStecilView,
		ID3D11Texture2D *_depthTexturePtr) {
		Destroy();
		bShouldDeleteDepth = false;

		depthTexturePtr = _depthTexturePtr;
		depthStecilView = _depthStecilView;
	}

	void RenderTarget::ActivateTarget(DWORD ignored) {
		// Implement a solution for the Deferred Renderer Lab

		size_t count = renderTargets.size();

		assert(count);
		ID3D11RenderTargetView *views[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];

		for ( size_t i = 0; i < count; ++i )
			views[i] = static_cast<RenderTargetSurface*>(renderTargets[i])->GetRenderTargetView();

		Renderer::theContextPtr->OMSetRenderTargets((UINT)count, views, depthStecilView);

		//return ActivateTargetSolution();
	}

	bool RenderTarget::AddTarget(RenderTargetSurface *view) {
		renderTargets.push_back(view);
		renderTargetsSRVs.push_back(view->GetShaderResourceView());
		return true;
	}

	void RenderTarget::Destroy() {
		renderTargets.clear();
		renderTargetsSRVs.clear();
	}

	void RenderTarget::ClearRenderTargetView(const FLOAT colorRGBA[4]) {
		size_t size = renderTargets.size();

		for ( size_t i = 0; i < size; ++i ) {
			Renderer::theContextPtr->ClearRenderTargetView(renderTargets[i]->GetRenderTargetView(),
				colorRGBA);
		}
	}

	void RenderTarget::ClearDepthStencilView(UINT clearFlags, FLOAT depth, UINT8 stencil) {
		if ( depthStecilView )
			Renderer::theContextPtr->ClearDepthStencilView(depthStecilView, clearFlags, depth,
				stencil);
	}

	void RenderTarget::ActivateSRVs(UINT startSlot) {
		Renderer::theContextPtr->PSSetShaderResources(startSlot, 4, &renderTargetsSRVs[0]);
		Renderer::theContextPtr->DSSetShaderResources(startSlot, 4, &renderTargetsSRVs[0]);
	}

	void RenderTarget::DeactivateSRVs(UINT startSlot) {
		ID3D11ShaderResourceView *nullSRVs[128] = { 0, };
		Renderer::theContextPtr->PSSetShaderResources(startSlot, 4, nullSRVs);
		Renderer::theContextPtr->DSSetShaderResources(startSlot, 4, nullSRVs);

	}
}