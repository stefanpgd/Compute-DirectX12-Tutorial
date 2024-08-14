#include "Graphics/DXStructuredBuffer.h"
#include "Graphics/DXUtilities.h"

DXStructuredBuffer::DXStructuredBuffer(const void* data, unsigned int numberOfElements, unsigned int elementSize)
	: numberOfElements(numberOfElements), elementSize(elementSize)
{
	DXDescriptorHeap* heap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvIndex = heap->GetNextAvailableIndex();
	uavIndex = heap->GetNextAvailableIndex();

	bufferSize = numberOfElements * elementSize;

	UpdateData(data);
}

void DXStructuredBuffer::UpdateData(const void* data)
{
	DXDescriptorHeap* heap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_RESOURCE_DESC description = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, 0);

	D3D12_SUBRESOURCE_DATA subresource;
	subresource.pData = data;
	subresource.RowPitch = bufferSize;

	ComPtr<ID3D12Resource> intermediate;
	UploadPixelShaderResource(structuredBuffer, intermediate, description, subresource);

	// Create SRV //
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.NumElements = numberOfElements;
	srvDesc.Buffer.StructureByteStride = elementSize;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	DXAccess::GetDevice()->CreateShaderResourceView(structuredBuffer.Get(), &srvDesc, heap->GetCPUHandleAt(srvIndex));

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

ComPtr<ID3D12Resource> DXStructuredBuffer::GetResource()
{
	return structuredBuffer;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DXStructuredBuffer::GetSRV()
{
	DXDescriptorHeap* SRVHeap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	return SRVHeap->GetGPUHandleAt(srvIndex);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DXStructuredBuffer::GetUAV()
{
	DXDescriptorHeap* UAVHeap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	return UAVHeap->GetGPUHandleAt(uavIndex);
}