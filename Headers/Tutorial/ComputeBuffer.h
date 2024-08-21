#pragma once
#include <d3dx12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

class ComputeBuffer
{
public:
	ComputeBuffer(unsigned int width, unsigned int height, DXGI_FORMAT format);

	ID3D12Resource* GetAddress();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetUAV();

private:
	void AllocateDataOnGPU();
	void CreateDescriptor();

private:
	ComPtr<ID3D12Resource> buffer;
	unsigned int uavIndex;

	unsigned int width;
	unsigned int height;
	DXGI_FORMAT format;
};