#pragma once

#include <Game/Resources/Resource.hpp>
#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>
#include <vector>
#include <cstdint>

template<typename T>
struct BufferProvider
{

	virtual int GetLength () const = 0;

	virtual const T * GetData () const = 0;

};

template<typename T>
struct Buffer : public BufferProvider<T>
{

	virtual inline const T * GetData () const override
	{
		return data.data ();
	}

	virtual inline int GetLength () const override
	{
		return static_cast<int>(data.size ());
	}

	std::vector<T> data;

};

class BufferResource : public SingleResource
{

public:

	virtual void ForceCreate (ID3D11Device & device) override;

	virtual void ForceDestroy () override;

	virtual bool IsCreated () const override;

protected:

	enum class Type
	{
		VertexBuffer, IndexBuffer
	};

	virtual Type GetType () const = 0;

	virtual size_t GetStride () const = 0;

	virtual const void * GetData () const = 0;

	virtual int GetLength () const = 0;

	ID3D11Buffer * GetBuffer () const;

private:

	ID3D11Buffer * m_pBuffer { nullptr };

};

class VertexBufferResource : public BufferResource
{

public:

	static void Set (ID3D11DeviceContext& deviceContext, int startingSlot, const std::vector<const VertexBufferResource*>& buffers);

	void Set (ID3D11DeviceContext& deviceContext, int slot);

protected:

	virtual inline Type GetType () const override final
	{
		return Type::VertexBuffer;
	}

};

template<typename T>
class TVertexBufferResource : public VertexBufferResource
{

public:

	const BufferProvider<T> * pProvider { nullptr };

protected:

	virtual size_t GetStride () const override
	{
		return sizeof (T);
	}

	virtual const void * GetData () const override
	{
		return pProvider->GetData ();
	}

	virtual int GetLength () const override
	{
		return pProvider->GetLength ();
	}

};

template<bool bHalfPrecision>
class IndexBufferResource : public BufferResource
{

public:

	void Set (ID3D11DeviceContext& _deviceContext) const
	{
		GAME_ASSERT_MSG (IsCreated (), "Not created");
		GAME_COMC (_deviceContext.IASetIndexBuffer (GetBuffer (), bHalfPrecision ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0));
	}

	using ind_t = std::conditional<bHalfPrecision, uint16_t, uint32_t>;

	const BufferProvider<ind_t> * pProvider { nullptr };

protected:

	virtual size_t GetStride () const override
	{
		return sizeof (ind_t);
	}

	virtual const void * GetData () const override
	{
		return pProvider->GetData ();
	}

	virtual int GetLength () const override
	{
		return pProvider->GetLength ();
	}

	virtual Type GetType () const override
	{
		return Type::IndexBuffer;
	}

};