#pragma once

#include "Graphics/DXCommon.h"

class ComputeRenderingStage
{
public:
	ComputeRenderingStage();

	void RecordStage(ComPtr<ID3D12GraphicsCommandList4> commandList);

private:
	void InitializeResources();
	void InitializePipeline();

private:
	// Insert Tutorial Variables //
};