#include "Graphics/DXComputePipeline.h"
#include "Graphics/DXRootSignature.h"
#include "Graphics/DXUtilities.h"

#include "Utilities/Logger.h"
#include <d3dcompiler.h>
#include <cassert>

DXComputePipeline::DXComputePipeline(DXRootSignature* rootSignature, std::string computePath)
	: rootSignature(rootSignature)
{
	CompileShaders(computePath);
	CreatePipelineState();
}

ComPtr<ID3D12PipelineState> DXComputePipeline::Get()
{
	return pipeline;
}

ID3D12PipelineState* DXComputePipeline::GetAddress()
{
	return pipeline.Get();
}

void DXComputePipeline::CompileShaders(std::string computePath)
{
	ComPtr<ID3DBlob> computeError;
	std::wstring computeFilePath(computePath.begin(), computePath.end());

	D3DCompileFromFile(computeFilePath.c_str(), NULL, NULL, "main", "cs_5_1", 0, 0, &computeShaderBlob, &computeError);

	if(!computeError == NULL)
	{
		std::string buffer = std::string((char*)computeError->GetBufferPointer());
		LOG(Log::MessageType::Error, buffer);
		assert(false && "Compilation of shader failed, read console for errors.");
	}
}

void DXComputePipeline::CreatePipelineState()
{
	// Any object in the StateStream is considered a 'Token'
	// Any token in this struct will automatically be implemented in the PSO
	// For example: If the token 'Stream Output' isn't present, it won't be compiled with the PSO.
	struct PipelineStateStream
	{
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE RootSignature;
		CD3DX12_PIPELINE_STATE_STREAM_CS CS;
	} PSS;

	PSS.RootSignature = rootSignature->GetAddress();
	PSS.CS = CD3DX12_SHADER_BYTECODE(computeShaderBlob.Get());

	D3D12_PIPELINE_STATE_STREAM_DESC pssDescription = { sizeof(PSS), &PSS };
	ThrowIfFailed(DXAccess::GetDevice()->CreatePipelineState(&pssDescription, IID_PPV_ARGS(&pipeline)));
}