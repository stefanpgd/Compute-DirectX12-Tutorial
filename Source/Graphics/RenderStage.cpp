#include "Graphics/RenderStage.h"
#include "Graphics/DXAccess.h"

RenderStage::RenderStage()
{ 
	window = DXAccess::GetWindow();
}