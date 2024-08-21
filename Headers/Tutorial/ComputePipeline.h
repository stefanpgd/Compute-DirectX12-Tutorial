#pragma once
#include <string>
#include <d3dx12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

class DXRootSignature;

class ComputePipeline
{
public:
	ComputePipeline(DXRootSignature* rootSignature, const std::string& shaderFilePath);

	ID3D12PipelineState* GetAddress();

private:
	void CompileShaders(const std::string& shaderFilePath);
	void CreatePipelineState(DXRootSignature* rootSignature);

private:
	ComPtr<ID3D12PipelineState> pipeline;
	ComPtr<ID3DBlob> computeShaderBlob;
};