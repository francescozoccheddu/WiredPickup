#pragma once

#include <Game/Direct3D.hpp>
#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/NonCopyable.hpp>

#define GAME_RESOURCE_ASSERT_CREATED {GAME_ASSERT_MSG(IsCreated(),"Resource not created");}
#define GAME_RESOURCE_ASSERT_NOTCREATED {GAME_ASSERT_MSG(!IsCreated(),"Resource already created");}

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

public:

	virtual bool IsCreated () const = 0;

	virtual void EnsureCreate (ID3D11Device & device) override final;

	virtual void EnsureDestroy () override final;

};

template<typename T>
class SimpleResource : public AtomicResource
{

public:

	virtual void ForceCreate (ID3D11Device & _device) override final
	{
		GAME_RESOURCE_ASSERT_NOTCREATED;
		m_pResource = Create (_device);
	}

	virtual void ForceDestroy () override final
	{
		GAME_RESOURCE_ASSERT_CREATED;
		m_pResource->Release ();
		m_pResource = nullptr;
	}

	virtual bool IsCreated () const override final
	{
		return m_pResource;
	}

protected:

	T * GetPointer () const
	{
		GAME_RESOURCE_ASSERT_CREATED;
		return m_pResource;
	}

	virtual T * Create (ID3D11Device& device) const = 0;

private:

	T * m_pResource { nullptr };

};