#pragma once

class FlatGraphics;

class GraphicsResource
{
public:
	GraphicsResource() = default;
	~GraphicsResource() = default;

protected:
	static ID3D11Device* GetDevice(FlatGraphics& graphics);
	static ID3D11DeviceContext* GetContext(FlatGraphics& graphics);
};

