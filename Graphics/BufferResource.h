#pragma once

class FlatGraphics;

class BufferResource
{
public:
	virtual ~BufferResource() = default;

	virtual void BindBuffer(FlatGraphics&) abstract;
	virtual void BindBuffer(FlatGraphics&, BufferResource*) abstract;
	virtual void Clear(FlatGraphics&) abstract;
};

