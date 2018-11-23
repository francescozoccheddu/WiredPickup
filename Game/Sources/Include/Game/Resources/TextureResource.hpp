#pragma once

#include <Game/Resources/Resource.hpp>

class TextureResource1D : public SimpleResource<ID3D11Texture1D>
{

public:

	using SimpleResource::GetPointer;

	bool bInitialize { false };
	D3D11_TEXTURE1D_DESC description;
	D3D11_SUBRESOURCE_DATA initialData;

private:

	virtual ID3D11Texture1D * Create (ID3D11Device & device) const override final;

};

class TextureResource2D : public SimpleResource<ID3D11Texture2D>
{

public:

	using SimpleResource::GetPointer;

	bool bInitialize { false };
	D3D11_TEXTURE2D_DESC description;
	D3D11_SUBRESOURCE_DATA initialData;

private:

	virtual ID3D11Texture2D * Create (ID3D11Device & device) const override final;

};

class TextureResource3D : public SimpleResource<ID3D11Texture3D>
{

public:

	using SimpleResource::GetPointer;

	bool bInitialize { false };
	D3D11_TEXTURE3D_DESC description;
	D3D11_SUBRESOURCE_DATA initialData;

private:

	virtual ID3D11Texture3D * Create (ID3D11Device & device) const override final;

};