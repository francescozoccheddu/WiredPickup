#pragma once

#include <Game/Resources/Resource.hpp>

class RasterizerStateResource : public AtomicResource
{

public:

	virtual void ForceCreate (ID3D11Device & device) override;

	virtual void ForceDestroy () override;

	virtual bool IsCreated () const override;

	D3D11_RASTERIZER_DESC description;

private:

	ID3D11RasterizerState * m_pRasterizerState;

};