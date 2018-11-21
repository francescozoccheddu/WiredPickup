#include <Game/Resources/ResourceGroup.h>

void ResourceGroup::ForceCreate (ID3D11Device & _device)
{
	for (Resource * pResource : resources)
	{
		pResource->ForceCreate (_device);
	}
}

void ResourceGroup::ForceDestroy ()
{
	for (Resource * pResource : resources)
	{
		pResource->ForceDestroy ();
	}
}

void ResourceGroup::EnsureCreate (ID3D11Device & _device)
{
	for (Resource * pResource : resources)
	{
		pResource->EnsureCreate (_device);
	}
}

void ResourceGroup::EnsureDestroy ()
{
	for (Resource * pResource : resources)
	{
		pResource->EnsureDestroy ();
	}
}
