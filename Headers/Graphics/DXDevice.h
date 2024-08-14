#pragma once

#include "Graphics/DXCommon.h"

class DXDevice
{
public:
	DXDevice(bool checkForRayTracingSupport = false);

	ComPtr<ID3D12Device5> Get();
	ID3D12Device5* GetAddress();

private:
	void DebugLayer();
	void SetupMessageSeverities();

	ComPtr<ID3D12Device5> device;
};