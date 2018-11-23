#include <Game/Resources/TextureResource.hpp>

#include <Game/Utils/COMExceptions.hpp>

ID3D11Texture1D * TextureResource1D::Create (ID3D11Device & _device) const
{
	ID3D11Texture1D * pResource;
	GAME_COMC (_device.CreateTexture1D (&description, bInitialize ? &initialData : nullptr, &pResource));
	return pResource;
}

ID3D11Texture2D * TextureResource2D::Create (ID3D11Device & _device) const
{
	ID3D11Texture2D * pResource;
	GAME_COMC (_device.CreateTexture2D (&description, bInitialize ? &initialData : nullptr, &pResource));
	return pResource;
}

ID3D11Texture3D * TextureResource3D::Create (ID3D11Device & _device) const
{
	ID3D11Texture3D * pResource;
	GAME_COMC (_device.CreateTexture3D (&description, bInitialize ? &initialData : nullptr, &pResource));
	return pResource;
}
