#include <Game/Resources/ResourceGroup.hpp>

#define _FOREACH(m) { for (Resource * pRes : resources) { pRes -> m ; } }

void ResourceGroup::ForceCreate (ID3D11Device & _device) _FOREACH (ForceCreate (_device))

void ResourceGroup::ForceDestroy () _FOREACH (ForceDestroy ())

void ResourceGroup::EnsureCreate (ID3D11Device & _device) _FOREACH (EnsureCreate (_device))

void ResourceGroup::EnsureDestroy () _FOREACH (EnsureDestroy ())

