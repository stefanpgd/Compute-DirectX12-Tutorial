#include "Graphics/DXUploadBuffer.h"
#include "Graphics/DXUtilities.h"
#include "Graphics/DXDescriptorHeap.h"

DXUploadBuffer::DXUploadBuffer(unsigned int size) : bufferSize(size)
{
	ComPtr<ID3D12Device5> device = DXAccess::GetDevice();
	CD3DX12_HEAP_PROPERTIES heapDesc = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC instanceResource = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	ThrowIfFailed(device->CreateCommittedResource(&heapDesc, D3D12_HEAP_FLAG_NONE, &instanceResource,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&buffer)));

	CreateDescriptor();
}

DXUploadBuffer::DXUploadBuffer(void* data, unsigned int size) : bufferSize(size)
{
	AllocateAndMapResource(buffer, data, bufferSize);
	CreateDescriptor();
}

void DXUploadBuffer::UpdateData(void* data)
{
	UpdateUploadHeapResource(buffer, data, bufferSize);
}

D3D12_GPU_VIRTUAL_ADDRESS DXUploadBuffer::GetGPUVirtualAddress()
{
	return buffer->GetGPUVirtualAddress();
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DXUploadBuffer::GetCBV()
{
	DXDescriptorHeap* CBVHeap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	return CBVHeap->GetGPUHandleAt(cbvIndex);
}

unsigned int DXUploadBuffer::GetCBVIndex()
{
	return cbvIndex;
}

unsigned int DXUploadBuffer::GetBufferSize()
{
	return bufferSize;
}

void DXUploadBuffer::CreateDescriptor()
{
	ComPtr<ID3D12Device5> device = DXAccess::GetDevice();
	DXDescriptorHeap* cbvHeap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	cbvIndex = cbvHeap->GetNextAvailableIndex();
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = cbvHeap->GetCPUHandleAt(cbvIndex);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = buffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = bufferSize;

	device->CreateConstantBufferView(&cbvDesc, handle);
}