#pragma once

#include <Game/Resources/Resource.hpp>

#include <Game/Utils/WindowRect.hpp>

class DepthBufferResource : public AtomicResource
{

public:

	virtual void ForceCreate (ID3D11Device & device) override;

	virtual void ForceDestroy () override;

	virtual bool IsCreated () const override;

	ID3D11DepthStencilView * Get () const;

	void Clear (ID3D11DeviceContext& deviceContext, float value);

	WindowSize size { 2, 2 };
	bool bHalfPrecision { false };

private:

	ID3D11DepthStencilView * m_pDepthView;

};
