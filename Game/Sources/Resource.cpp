#include <Game/Resources/Resource.hpp>

void SingleResource::EnsureCreate (ID3D11Device & _device)
{
	if (!IsCreated ())
	{
		ForceCreate (_device);
	}
}

void SingleResource::EnsureDestroy ()
{
	if (IsCreated ())
	{
		ForceDestroy ();
	}
}
