#include <Game/Resources/ShaderResourceViewResource.hpp>

ID3D11ShaderResourceView * ShaderResourceViewResource::Create (ID3D11Device & _device) const
{
	return nullptr;
}

ID3D11Resource * ResourceWrapper::GetResource () const
{
	GAME_ASSERT_MSG (pResource, "Resource is nullptr");
	return pResource;
}
