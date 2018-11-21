#pragma once

#include <Game/Resources/Resource.hpp>
#include <unordered_set>

class ResourceGroup : public Resource
{

public:

	virtual void ForceCreate (ID3D11Device & device) override;

	virtual void ForceDestroy () override;

	virtual void EnsureCreate (ID3D11Device & _device) override;

	virtual void EnsureDestroy () override;

	std::unordered_set<Resource*> resources;

};