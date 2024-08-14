#pragma once

#include "Graphics/DXCommon.h"

class Texture
{
public:
	Texture(int width, int height, DXGI_FORMAT format);
	Texture(void* data, int width, int height,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, unsigned int formatSizeInBytes = 4);
	~Texture();

	int GetWidth();
	int GetHeight();
	DXGI_FORMAT GetFormat();

	int GetSRVIndex();
	int GetUAVIndex();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetSRV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetUAV();

	ID3D12Resource* GetAddress();
	ComPtr<ID3D12Resource> GetResource();
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();

private:
	void AllocateTexture();
	void UploadData(void* data);

	void CreateDescriptors();

private:
	ComPtr<ID3D12Resource> textureResource;

	DXGI_FORMAT format;
	unsigned int formatSizeInBytes;
	int width;
	int height;

	int srvIndex = 0;
	int uavIndex = 0;
};