#include "Tutorial/StructuredBuffer.h"
#include "Graphics/DXUtilities.h"

StructuredBuffer::StructuredBuffer(const void* data, unsigned int numberOfElements, unsigned int elementSize)
{
	DXDescriptorHeap* heap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	uavIndex = heap->GetNextAvailableIndex();

	UpdateData(data, numberOfElements, elementSize);
}

void StructuredBuffer::UpdateData(const void* data, unsigned int numberOfElements, unsigned int elementSize)
{
	unsigned int bufferSize = numberOfElements * elementSize;

	DXDescriptorHeap* heap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_RESOURCE_DESC description = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, 0);

	D3D12_SUBRESOURCE_DATA subresource;
	subresource.pData = data;
	subresource.RowPitch = bufferSize;

	ComPtr<ID3D12Resource> intermediate;
	UploadPixelShaderResource(structuredBuffer, intermediate, description, subresource);

	// Create UAV //
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.NumElements = numberOfElements;
	uavDesc.Buffer.StructureByteStride = elementSize;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

	DXAccess::GetDevice()->CreateUnorderedAccessView(structuredBuffer.Get(), nullptr, &uavDesc, heap->GetCPUHandleAt(uavIndex));
}

ComPtr<ID3D12Resource> StructuredBuffer::GetResource()
{
	return structuredBuffer;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE StructuredBuffer::GetUAV()
{
	DXDescriptorHeap* UAVHeap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	return UAVHeap->GetGPUHandleAt(uavIndex);
}