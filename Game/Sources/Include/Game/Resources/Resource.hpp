#pragma once

#include <Game/Direct3D.hpp>
#include <Game/Utils/NonCopyable.hpp>

class Resource : private NonCopyable
{

public:

	virtual void Create (ID3D11Device & device) = 0;

	virtual void Destroy () = 0;

	virtual bool IsCreated () const = 0;

};
