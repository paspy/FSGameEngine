#include "precompiled.h"
#include "Renderer.h"
#include "RenderShapeWithTexture.h"

using namespace EDRendererD3D;
using namespace DirectX;
using namespace EDUtilities;

void RenderShapeWithTexture::Initialize(std::vector< ContentHandle<RenderForm> >&formHandles,
	XMFLOAT4X4* localMatrixPtr) {
	SetRenderFormHandle(formHandles);

	if ( localMatrixPtr != 0 )
		SetWorldMatrix(*localMatrixPtr);

	RenderFunc = IndexedPrimitiveRenderFunc;
}

void RenderShapeWithTexture::SetTextures(RenderShapeWithTexture &shape) {
	// Implement a solution for the Renderer Lab
	//RenderMaterial::GetRTHandles()
	RenderMaterial* rm = shape.GetRenderMaterial();
	RenderContext* rc = shape.GetRenderContext();

	
	ID3D11ShaderResourceView *arr[6];
	ZeroMemory(arr, sizeof(ID3D11ShaderResourceView) * 6);

	for ( size_t i = 0; i < rm->GetRTHandles().size(); i++ ) {
		if ( rm->GetRTHandles()[i].GetContent() ) {
			arr[i] = rm->GetRTHandles()[i].GetContent()->GetShaderResourceView();
		} else {
			arr[i] = nullptr;
		}
	}

	if ( rm->GetRTCubeMapHandle().GetContent() ) {
		arr[5] = rm->GetRTCubeMapHandle().GetContent()->GetShaderResourceView();
	}
	rc->EffectSetPerObjectTextures(arr);
	//SetTexturesSolution(shape);
}

void RenderShapeWithTexture::IndexedPrimitiveRenderFunc(RenderNode &node) {
	// Implement a solution for the Renderer Lab
	RenderShapeWithTexture &shapeNode = (RenderShapeWithTexture&)node;

	SetTextures(shapeNode);
	RenderShape::IndexedPrimitiveRenderFunc(node);
	//return WithTextureIndexedPrimitiveRenderFuncSolution(node);
}
