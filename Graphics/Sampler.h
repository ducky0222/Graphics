#pragma once

#include "IBindable.h"

class Sampler : public IBindable
{
public:
	enum class Type
	{
		ANISOTROPIC,
		BILINEAR
	};
};

