#include <Game/Resources/Resource.hpp>

void AtomicResource::EnsureCreate (ID3D11Device & _device)
{
	if (!IsCreated ())
	{
		ForceCreate (_device);
	}
}

void AtomicResource::EnsureDestroy ()
{
	if (IsCreated ())
	{
		ForceDestroy ();
	}
}
