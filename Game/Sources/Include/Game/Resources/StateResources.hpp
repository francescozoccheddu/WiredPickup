#pragma once

#include <Game/Resources/Resource.hpp>

#include <Game/Resources/ShaderType.hpp>
#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

class RasterizerStateResource : public SimpleResource<ID3D11RasterizerState>
{

public:

	static void Reset (ID3D11DeviceContext& deviceContext);

	D3D11_RASTERIZER_DESC description;

	void Set (ID3D11DeviceContext& deviceContext) const;

private:

	virtual ID3D11RasterizerState * Create (ID3D11Device & device) const override final;

};

class DepthStencilStateResource : public SimpleResource<ID3D11DepthStencilState>
{

public:

	static void Reset (ID3D11DeviceContext& deviceContext);

	D3D11_DEPTH_STENCIL_DESC description;

	void Set (ID3D11DeviceContext& deviceContext) const;

private:

	virtual ID3D11DepthStencilState * Create (ID3D11Device & device) const override final;

};

class BlendStateResource : public SimpleResource<ID3D11BlendState>
{

public:

	static void Reset (ID3D11DeviceContext& deviceContext, const FLOAT * factor, UINT sampleMask);

	D3D11_BLEND_DESC description;
	FLOAT factor[4];
	UINT sampleMask;

	void Set (ID3D11DeviceContext& deviceContext) const;

private:

	virtual ID3D11BlendState * Create (ID3D11Device & device) const override final;

};

class SamplerStateResource : public SimpleResource<ID3D11SamplerState>
{

public:

	static void Set (ID3D11DeviceContext& deviceContext, UINT startingSlot, ShaderType shaderType, const SamplerStateResource*const* pSamplers, int cSampler);

	void Set (ID3D11DeviceContext& deviceContext, UINT slot, ShaderType shaderType) const;

	D3D11_SAMPLER_DESC description;

private:

	virtual ID3D11SamplerState * Create (ID3D11Device & device) const override final;

	static void Set (ID3D11DeviceContext& deviceContext, UINT startingSlot, ShaderType shaderType, ID3D11SamplerState*const* pSamplerStates, int cSamplerStates);

};