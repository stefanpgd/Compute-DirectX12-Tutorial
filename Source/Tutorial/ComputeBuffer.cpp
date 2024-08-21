#include "Tutorial/ComputeBuffer.h"
#include "Graphics/DXAccess.h" // Part of the framework to retrieve things like Device or Descriptor Heap(s)
#include "Graphics/DXDescriptorHeap.h"

ComputeBuffer::ComputeBuffer(unsigned int width, unsigned int height, DXGI_FORMAT format) :
    width(width), height(height), format(format)
{
    AllocateDataOnGPU();
    CreateDescriptor();
}

ID3D12Resource* ComputeBuffer::GetAddress()
{
    return buffer.Get();
}

CD3DX12_GPU_DESCRIPTOR_HANDLE ComputeBuffer::GetUAV()
{
    DXDescriptorHeap* UAVHeap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    return UAVHeap->GetGPUHandleAt(uavIndex);
}

void ComputeBuffer::AllocateDataOnGPU()
{
    // 1) Write a description of our resource //
    D3D12_RESOURCE_DESC bufferDescription = {};
    bufferDescription.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

    bufferDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    bufferDescription.Width = width;
    bufferDescription.Height = height;
    bufferDescription.Format = format;

    bufferDescription.MipLevels = 1;
    bufferDescription.DepthOrArraySize = 1;
    bufferDescription.SampleDesc.Count = 1;
    bufferDescription.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

    // 2) Allocate our buffer with the device using the resource description //
    CD3DX12_HEAP_PROPERTIES defaultHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    DXAccess::GetDevice()->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE,
        &bufferDescription, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&buffer));
}

void ComputeBuffer::CreateDescriptor()
{
    DXDescriptorHeap* UAVHeap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // Create UAV //
    D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = format;
    uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;

    uavIndex = UAVHeap->GetNextAvailableIndex();
    DXAccess::GetDevice()->CreateUnorderedAccessView(buffer.Get(), nullptr, &uavDesc, UAVHeap->GetCPUHandleAt(uavIndex));
}