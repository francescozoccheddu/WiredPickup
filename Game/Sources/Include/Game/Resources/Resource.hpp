#pragma once

#include <Game/Direct3D.hpp>
#include <Game/Utils/NonCopyable.hpp>

class Resource : private NonCopyable
{

public:

	virtual void ForceCreate (ID3D11Device & device) = 0;

	virtual void ForceDestroy () = 0;

	inline virtual void EnsureCreate (ID3D11Device & device) = 0;

	inline virtual void EnsureDestroy () = 0;

};

class AtomicResource : public Resource
{

	virtual bool IsCreated () const = 0;

	virtual void EnsureCreate (ID3D11Device & device) override;

	virtual void EnsureDestroy () override;

};