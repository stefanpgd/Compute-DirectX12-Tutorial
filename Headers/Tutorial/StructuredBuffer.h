#pragma once
#include <d3dx12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

class StructuredBuffer
{
public:
	StructuredBuffer(const void* data, unsigned int numberOfElements, unsigned int elementSize);

	void UpdateData(const void* data, unsigned int numberOfElements, unsigned int elementSize);

	ComPtr<ID3D12Resource> GetResource();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetUAV();

private:
	ComPtr<ID3D12Resource> structuredBuffer;

	int srvIndex = -1;
	int uavIndex = -1;
};