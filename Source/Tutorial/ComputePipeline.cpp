#include "Tutorial/ComputePipeline.h"
#include "Graphics/DXUtilities.h"
#include "Utilities/Logger.h"

ComputePipeline::ComputePipeline(DXRootSignature* rootSignature, const std::string& shaderFilePath) : rootSignature(rootSignature)
{
}

ID3D12PipelineState* ComputePipeline::GetAddress()
{
	return nullptr;
}

void ComputePipeline::CompileShaders(const std::string& shaderFilePath)
{
}

void ComputePipeline::CreatePipelineState()
{
}