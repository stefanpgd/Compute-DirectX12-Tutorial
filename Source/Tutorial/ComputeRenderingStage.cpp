#include "Tutorial/ComputeRenderingStage.h"
#include "Tutorial/ComputePipeline.h"
#include "Tutorial/ComputeBuffer.h"
#include "Tutorial/StructuredBuffer.h"

#include "Graphics/DXRootSignature.h"
#include "Graphics/Window.h"
#include "Graphics/DXAccess.h"
#include "Graphics/DXUtilities.h"

#include "Utilities/Random.h"

ComputeRenderingStage::ComputeRenderingStage()
{
	InitializeResources();
	InitializePipeline();
}

void ComputeRenderingStage::InitializeResources()
{
	unsigned int screenWidth = DXAccess::GetWindow()->GetWindowWidth();
	unsigned int screenHeight = DXAccess::GetWindow()->GetWindowHeight();

	backBuffer = new ComputeBuffer(screenWidth, screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM);

	for(int i = 0; i < 500; i++)
	{
		Particle particle;
		particle.position[0] = RandomInRange(0, screenWidth);
		particle.position[1] = RandomInRange(0, screenHeight);

		particle.color[0] = Random01();
		particle.color[1] = Random01();
		particle.color[2] = Random01();

		particles.push_back(particle);
	}

	StructuredBuffer* particleBuffer = new StructuredBuffer(particles.data(), particles.size(), sizeof(Particle));
}

void ComputeRenderingStage::InitializePipeline()
{
	CD3DX12_DESCRIPTOR_RANGE1 backBufferRange[1];
	backBufferRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0);

	CD3DX12_ROOT_PARAMETER1 pipelineParameters[1];
	pipelineParameters[0].InitAsDescriptorTable(1, &backBufferRange[0]);

	computeRootSignature = new DXRootSignature(pipelineParameters, _countof(pipelineParameters));
	computePipeline = new ComputePipeline(computeRootSignature, "Source/Shaders/hello.compute.hlsl");
}

void ComputeRenderingStage::RecordStage(ComPtr<ID3D12GraphicsCommandList4> commandList)
{
	// 1. Bind our root signature & pipeine //
	commandList->SetComputeRootSignature(computeRootSignature->GetAddress());
	commandList->SetPipelineState(computePipeline->GetAddress());

	// 2. Bind resources needed for our pipeline //
	commandList->SetComputeRootDescriptorTable(0, backBuffer->GetUAV());

	// 3. Dispatch our compute shader //
	unsigned int screenWidth = DXAccess::GetWindow()->GetWindowWidth();
	unsigned int screenHeight = DXAccess::GetWindow()->GetWindowHeight();
	unsigned int dispatchX = screenWidth / 8;
	unsigned int dispatchY = screenHeight / 8;

	commandList->Dispatch(dispatchX, dispatchY, 1);

	// 4. Copy the result of our back buffer into the screen buffer //
	ComPtr<ID3D12Resource> screenBuffer = DXAccess::GetWindow()->GetCurrentScreenBuffer();

	TransitionResource(screenBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_DEST);
	commandList->CopyResource(screenBuffer.Get(), backBuffer->GetAddress());
	TransitionResource(screenBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);
}