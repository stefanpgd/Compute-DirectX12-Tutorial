#pragma once
#include <d3d12.h>
#include <d3dx12.h>

#include <wrl.h>
using namespace Microsoft::WRL;

class ComputeRenderingStage
{
public:
	// The Constructor & 'RecordStage' functions are by default 
	// called by the Renderer.cpp, hence why there are already present in this file. 
	ComputeRenderingStage();

	void RecordStage(ComPtr<ID3D12GraphicsCommandList4> commandList);

	// Insert Tutorial Code //
};