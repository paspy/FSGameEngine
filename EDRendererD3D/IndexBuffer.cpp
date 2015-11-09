#include "precompiled.h"
#include "IndexBuffer.h"
#include "Renderer.h"


namespace EDRendererD3D
{
    IndexBuffer *IndexBuffer::instancePtr = 0;

    IndexBuffer::IndexBuffer(void)
    {
        indexBufferPtr = 0;
    }

    IndexBuffer::~IndexBuffer(void)
    {
		ReleaseCOM(indexBufferPtr);
    }

    IndexBuffer &IndexBuffer::GetReference()
    {
        if(!instancePtr)
        {
			instancePtr = new IndexBuffer;
        }
        return *instancePtr;
    }

    void IndexBuffer::DeleteInstance()
    {
		delete instancePtr;
    }

    UINT IndexBuffer::AddIndices(const UINT *_indices, UINT _numIndices)
    {
		UINT ret = 0;
	
		if ( indexBufferPtr ) {
			D3D11_BUFFER_DESC buffDesc;
			ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));
			indexBufferPtr->GetDesc(&buffDesc);
			UINT oldWidth = buffDesc.ByteWidth;
			UINT newWidth = oldWidth + _numIndices * sizeof(unsigned int);
			buffDesc.ByteWidth = newWidth;
			
			D3D11_SUBRESOURCE_DATA sData;
			sData.pSysMem = (char*)_indices - oldWidth;
			ID3D11Buffer* tmpBuffer = nullptr;

			Renderer::theDevicePtr->CreateBuffer(&buffDesc, &sData, &tmpBuffer);

			Renderer::theContextPtr->CopySubresourceRegion(tmpBuffer, 0, 0, 0, 0, indexBufferPtr, 0, 0);
			ReleaseCOM(indexBufferPtr);
			indexBufferPtr = tmpBuffer;

			ret = oldWidth / sizeof(unsigned int);
		
		} else {
			D3D11_BUFFER_DESC buffDesc;
			ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));

			buffDesc.Usage = D3D11_USAGE_DEFAULT;
			buffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			buffDesc.ByteWidth = _numIndices * sizeof(unsigned int);

			D3D11_SUBRESOURCE_DATA sData;
			sData.pSysMem = _indices;
			Renderer::theDevicePtr->CreateBuffer(&buffDesc, &sData, &indexBufferPtr);

			ret = 0;
		}
		//ret = AddIndicesSolution(_indices, _numIndices);

		return ret;
    }
}