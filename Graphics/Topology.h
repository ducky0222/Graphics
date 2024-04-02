#pragma once

#include "IBindable.h"

class Topology : public IBindable
{
public:
	Topology(FlatGraphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type);
	virtual ~Topology() = default;

	static std::shared_ptr<Topology> Create(FlatGraphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);

	// IBindable을(를) 통해 상속됨
	void Bind(FlatGraphics& graphics) override;

	std::string GetUID() const override;

protected:
	D3D11_PRIMITIVE_TOPOLOGY m_type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

