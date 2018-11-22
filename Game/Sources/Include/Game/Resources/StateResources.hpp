#pragma once

#include <Game/Resources/Resource.hpp>

#include <Game/Resources/ShaderType.hpp>
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

protected:

	T * GetState () const
	{
		GAME_ASSERT_MSG (IsCreated (), "Not created");
		return m_pState;
	}

	virtual T * Create (ID3D11Device& device) const = 0;

private:

	T * m_pState;

};

class RasterizerStateResource : public StateResource<ID3D11RasterizerState>
{

public:

	static void Reset (ID3D11DeviceContext& deviceContext);

	D3D11_RASTERIZER_DESC description;

	void Set (ID3D11DeviceContext& deviceContext) const;

protected:

	virtual ID3D11RasterizerState * Create (ID3D11Device & device) const override final;

};

class DepthStencilStateResource : public StateResource<ID3D11DepthStencilState>
{

public:

	static void Reset (ID3D11DeviceContext& deviceContext);

	D3D11_DEPTH_STENCIL_DESC description;

	void Set (ID3D11DeviceContext& deviceContext) const;

protected:

	virtual ID3D11DepthStencilState * Create (ID3D11Device & device) const override final;

};

class BlendStateResource : public StateResource<ID3D11BlendState>
{

public:

	static void Reset (ID3D11DeviceContext& deviceContext, const FLOAT * factor, UINT sampleMask);

	D3D11_BLEND_DESC description;
	FLOAT factor[4];
	UINT sampleMask;

	void Set (ID3D11DeviceContext& deviceContext) const;

protected:

	virtual ID3D11BlendState * Create (ID3D11Device & device) const override final;

};

class SamplerStateResource : public StateResource<ID3D11SamplerState>
{

public:

	static void Set (ID3D11DeviceContext& deviceContext, int startingSlot, ShaderType shaderType, const std::vector<const SamplerStateResource*> samplers);

	void Set (ID3D11DeviceContext& deviceContext, int slot, ShaderType shaderType) const;

	D3D11_SAMPLER_DESC description;

protected:

	virtual ID3D11SamplerState * Create (ID3D11Device & device) const override final;

};