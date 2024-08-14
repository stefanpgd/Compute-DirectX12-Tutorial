#pragma once

#include "DXCommon.h"

/// <summary>
/// A Buffer that resides on the UPLOAD heap of the GPU. Which allows us to directly
/// map data without having wait on the GPU. This class is mainly provides some ease-of-use 
/// utilities around those type of allocated resources.
/// </summary>
class DXUploadBuffer
{
public:
	DXUploadBuffer(unsigned int size);
	DXUploadBuffer(void* data, unsigned int size);

	void UpdateData(void* data);

	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetCBV();
	unsigned int GetCBVIndex();
	unsigned int GetBufferSize();

private:
	void CreateDescriptor();

private:
	ComPtr<ID3D12Resource> buffer;
	unsigned int bufferSize;

	unsigned int cbvIndex;
};