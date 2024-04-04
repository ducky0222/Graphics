#pragma once

class FlatGraphics;
class IDrawable;
class Step;

class Job
{
public:
	Job(const Step* step, IDrawable* drawable);
	~Job() = default;

	void Execute(FlatGraphics& graphics, bool hasAlpha, size_t layer) const;

private:
	IDrawable* m_drawable = nullptr;
	const Step* m_step = nullptr;
};

