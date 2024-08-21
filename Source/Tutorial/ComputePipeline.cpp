#include "Tutorial/ComputePipeline.h"
#include "Graphics/DXUtilities.h"
#include "Graphics/DXRootSignature.h"
#include "Utilities/Logger.h"

#include <d3dcompiler.h>
#include <cassert>

ComputePipeline::ComputePipeline(DXRootSignature* rootSignature, const std::string& shaderFilePath)
{
	CompileShaders(shaderFilePath);
	CreatePipelineState(rootSignature);
}

ID3D12PipelineState* ComputePipeline::GetAddress()
{
	return pipeline.Get();
}

void ComputePipeline::CompileShaders(const std::string& shaderFilePath)
{
	// 1) turn 'string' into 'wstring'
	ComPtr<ID3DBlob> computeError;
	std::wstring computeFilePath(shaderFilePath.begin(), shaderFilePath.end());

	// 2) Compile compute shader
	D3DCompileFromFile(computeFilePath.c_str(), NULL, NULL, "main", "cs_5_1", 0, 0, &computeShaderBlob, &computeError);

	// 3) Check if there were any errors, if so assert & check the console for the error message
	if(computeError != NULL)
	{
		std::string buffer = std::string((char*)computeError->GetBufferPointer());
		LOG(Log::MessageType::Error, buffer);
		assert(false && "Compilation of shader failed, read console for errors.");
	}
}

void ComputePipeline::CreatePipelineState(DXRootSignature* rootSignature)
{
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