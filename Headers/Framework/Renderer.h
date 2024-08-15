#pragma once
#include <string>
#include <d3d12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

class ComputeRenderingStage;

class Renderer
{
public:
	Renderer(const std::wstring& applicationName, unsigned int windowWidth, unsigned int windowHeight);

	void Update(float deltaTime);
	void Render();
	void Resize();

private:
	void InitializeImGui();

	// Tutorial Code //
	void InitializeComputePipeline();
	void RecordComputePipeline(ComPtr<ID3D12GraphicsCommandList4> commandList);

private:
	ComputeRenderingStage* computeRenderingStage;
};