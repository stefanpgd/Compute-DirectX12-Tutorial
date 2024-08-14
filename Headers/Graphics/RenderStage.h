#pragma once

#include "Graphics/DXCommon.h"

class Window;
class DXRootSignature;

class DXPipeline;
class DXComputePipeline;

/// <summary>
/// The purpose of the RenderStage is to be able to encapsulate
/// render pass functionality. Whenever you need to do something
/// relating to screen-spaced pass, we go to ScreenStage.cpp
/// This also allows us to easily order our render passes in the Renderer.cpp
/// </summary>
class RenderStage
{
public:
	RenderStage();

	virtual void RecordStage(ComPtr<ID3D12GraphicsCommandList4> commandList) = 0;

protected:
	DXPipeline* pipeline;
	DXComputePipeline* computePipeline;
	DXRootSignature* rootSignature;

	Window* window;
};