#pragma once

#include "Graphics/DXCommon.h"

class ComputeBuffer
{
public:
	ComputeBuffer(int width, int height, DXGI_FORMAT format);

	ID3D12Resource* GetAddress();

	// Insert Tutorial Functions //

private:
	void AllocateDataOnGPU();
	void CreateDescriptors();

private:
	// Insert Tutorial Variables //
};