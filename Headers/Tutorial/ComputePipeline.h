#pragma once

#include "Graphics/DXCommon.h"
#include <string>

class ComputePipeline
{
public:
	ComputePipeline(const std::string& shaderFilePath);

	ID3D12PipelineState* GetAddress();

private:
	void CompileShaders(const std::string& shaderFilePath);
	void CreatePipelineState();

private:
	// Insert Tutorial Variables //
};