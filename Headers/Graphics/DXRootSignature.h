#pragma once

#include "Graphics/DXCommon.h"

class DXRootSignature
{
public:
	DXRootSignature(CD3DX12_ROOT_PARAMETER1* rootParameters, const unsigned int numberOfParameters,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

	ComPtr<ID3D12RootSignature> Get();
	ID3D12RootSignature* GetAddress();

private:
	ComPtr<ID3D12RootSignature> rootSignature;
};