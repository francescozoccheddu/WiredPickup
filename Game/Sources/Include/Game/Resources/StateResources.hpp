#pragma once

#include <Game/Resources/Resource.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

template<typename T>
class StateResource : public AtomicResource
{
public:

	virtual void ForceCreate (ID3D11Device & _device) override
	{
		GAME_ASSERT_MSG (!IsCreated (), "Already created");
		m_pState = Create (_device);
	}

	virtual void ForceDestroy () override
	{
		GAME_ASSERT_MSG (IsCreated (), "Not created");
		m_pState->Release ();
		m_pState = nullptr;
	}

	virtual bool IsCreated () const override
	{
		return m_pState;
	}

	void Set (ID3D11DeviceContext& _deviceContext) const
	{
		GAME_ASSERT_MSG (IsCreated (), "Not created");
		ForceSet (_deviceContext);
	}

protected:

	virtual void ForceSet (ID3D11DeviceContext& deviceContext, T * pState) const = 0;

	virtual T * Create (ID3D11Device& device) const = 0;

private:

	T * m_pState;

};

class RasterizerStateResource : public StateResource<ID3D11RasterizerState>
{

public:

	static void Reset (ID3D11DeviceContext& deviceContext);

	D3D11_RASTERIZER_DESC description;

protected:

	virtual void ForceSet (ID3D11DeviceContext& deviceContext, ID3D11RasterizerState * pState) const override final;

	virtual ID3D11RasterizerState * Create (ID3D11Device & device) const override final;

};

class DepthStencilStateResource : public StateResource<ID3D11DepthStencilState>
{

public:

	static void Reset (ID3D11DeviceContext& deviceContext);

	D3D11_DEPTH_STENCIL_DESC description;

protected:

	virtual void ForceSet (ID3D11DeviceContext& deviceContext, ID3D11DepthStencilState * pState) const override final;

	virtual ID3D11DepthStencilState * Create (ID3D11Device & device) const override final;

};

class BlendStateResource : public StateResource<ID3D11BlendState>
{

public:

	static void Reset (ID3D11DeviceContext& deviceContext, const FLOAT * factor, UINT sampleMask);

	D3D11_BLEND_DESC description;
	FLOAT factor[4];
	UINT sampleMask;

protected:

	virtual void ForceSet (ID3D11DeviceContext& deviceContext, ID3D11BlendState * pState) const override final;

	virtual ID3D11BlendState * Create (ID3D11Device & device) const override final;

};