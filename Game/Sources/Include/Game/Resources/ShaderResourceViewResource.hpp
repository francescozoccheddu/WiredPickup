#pragma once

#include <Game/Resources/Resource.hpp>
#include <Game/Resources/ShaderType.hpp>
#include <vector>

class ResourceProvider
{

public:

	virtual ID3D11Resource * GetResource () const = 0;

};

class ResourceWrapper : public ResourceProvider
{

public:

	virtual ID3D11Resource * GetResource () const override final;

	ID3D11Resource * pResource { nullptr };

};

class ShaderResourceViewResource : public SimpleResource<ID3D11ShaderResourceView>
{

public:

	using SimpleResource::GetPointer;

	static void Set (ID3D11DeviceContext& context, int startingSlot, ShaderType shaderType, const std::vector<const ShaderResourceViewResource*> resources);

	void Set (ID3D11DeviceContext& context, int slot, ShaderType shaderType);

	D3D11_SHADER_RESOURCE_VIEW_DESC description;
	const ResourceProvider * pProvider { nullptr };

private:

	virtual ID3D11ShaderResourceView * Create (ID3D11Device & device) const override final;

};