#include "pch.h"
#include "GraphicsResource.h"
#include "FlatGraphics.h"

ID3D11Device* GraphicsResource::GetDevice(FlatGraphics& graphics)
{
	return graphics.GetDevice().get();
}

ID3D11DeviceContext* GraphicsResource::GetContext(FlatGraphics& graphics)
{
	return graphics.GetContext().get();
}
