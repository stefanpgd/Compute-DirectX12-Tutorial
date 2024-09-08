#include "Tutorial/ComputeRenderingStage.h"
#include "Tutorial/ComputePipeline.h"
#include "Tutorial/ComputeBuffer.h"
#include "Tutorial/StructuredBuffer.h"

#include "Graphics/DXRootSignature.h"
#include "Graphics/Window.h"
#include "Graphics/DXAccess.h"
#include "Graphics/DXUtilities.h"

#include "Framework/Mathematics.h"
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

		particle.color = Random01();

		glm::vec2 velocity = glm::normalize(glm::vec2(RandomInRange(-1.0f, 1.0f), RandomInRange(-1.0f, 1.0f)));
		particle.velocity[0] = velocity.x;
		particle.velocity[1] = velocity.y;

		particles.push_back(particle);
	}

	particleBuffer = new StructuredBuffer(particles.data(), particles.size(), sizeof(Particle));
}

void ComputeRenderingStage::InitializePipeline()
{
	CD3DX12_DESCRIPTOR_RANGE1 backBufferRange[1];
	backBufferRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0);

	CD3DX12_DESCRIPTOR_RANGE1 particleBufferRange[1];
	particleBufferRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1, 0);

	CD3DX12_ROOT_PARAMETER1 pipelineParameters[2];
	pipelineParameters[0].InitAsDescriptorTable(1, &backBufferRange[0]);
	pipelineParameters[1].InitAsDescriptorTable(1, &particleBufferRange[0]);

	computeRootSignature = new DXRootSignature(pipelineParameters, _countof(pipelineParameters));
	computePipeline = new ComputePipeline(computeRootSignature, "Source/Shaders/hello.compute.hlsl");
}

void ComputeRenderingStage::RecordStage(ComPtr<ID3D12GraphicsCommandList4> commandList)
{
	// 1. Bind our root signature & pipeline //
	commandList->SetComputeRootSignature(computeRootSignature->GetAddress());
	commandList->SetPipelineState(computePipeline->GetAddress());

	// 2. Bind resources needed for our pipeline //
	commandList->SetComputeRootDescriptorTable(0, backBuffer->GetUAV());
	commandList->SetComputeRootDescriptorTable(1, particleBuffer->GetUAV());

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