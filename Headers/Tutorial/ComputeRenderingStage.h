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
	ComputeRenderingStage();

	void RecordStage(ComPtr<ID3D12GraphicsCommandList4> commandList);

private:
	void InitializeResources();
	void InitializePipeline();

private:
	DXRootSignature* computeRootSignature;
	ComputePipeline* computePipeline;

	ComputeBuffer* backBuffer;
};