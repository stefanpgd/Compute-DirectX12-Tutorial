#pragma once
#include <d3dx12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

class DXRootSignature;
class ComputePipeline;
class ComputeBuffer;

class ComputeRenderingStage
{
public:
	// The Constructor & 'RecordStage' functions will already be called by Renderer.cpp.
	// This is why they are already present in this file.
	ComputeRenderingStage();

	void RecordStage(ComPtr<ID3D12GraphicsCommandList4> commandList);

	// Insert Tutorial Code //
};