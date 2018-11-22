#pragma once

#include <Game/Resources/Resource.hpp>
#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>
#include <vector>
#include <cstdint>

class BufferResource : public SimpleResource<ID3D11Buffer>
{

public:

	using SimpleResource::GetPointer;

	D3D11_BUFFER_DESC description;
	D3D11_SUBRESOURCE_DATA data;
	bool bInitialize { false };

private:

	virtual ID3D11Buffer * Create (ID3D11Device & device) const override final;

};

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

class IABufferResourceBase : public AtomicResource
{

public:

	virtual void ForceCreate (ID3D11Device & device) override final;

	virtual void ForceDestroy () override final;

	virtual bool IsCreated () const override final;

protected:

	virtual void Prepare (BufferResource& buffer) = 0;

	ID3D11Buffer * GetPointer () const;

private:

	BufferResource m_Buffer;

};

class IndexBufferResourceBase : public IABufferResourceBase
{

public:

	static void Reset (ID3D11DeviceContext& deviceContext);

	void Set (ID3D11DeviceContext& deviceContext) const;

protected:

	virtual DXGI_FORMAT GetFormat () const = 0;

};

template<unsigned int bytePrecision>
class StaticIndexBufferResource : public IndexBufferResourceBase
{

public:

	static_assert(bytePrecision == 1 || bytePrecision == 2 || bytePrecision == 4, "Unsupported precision");

	using ind_t = std::conditional<bytePrecision == 1, uint8_t, std::conditional<bytePrecision == 2, uint16_t, uint32_t>>;

	int GetLength () const
	{
		GAME_ASSERT_MSG (pProvider, "Buffer provider is nullptr");
		return pProvider->GetLength ();
	}

	const BufferProvider<ind_t> * pProvider;

private:

	virtual DXGI_FORMAT GetFormat () const override final
	{
		switch (bytePrecision)
		{
			case 1:
				return DXGI_FORMAT_R8_UINT;
			case 2:
				return DXGI_FORMAT_R16_UINT;
			case 4:
				return DXGI_FORMAT_R32_UINT;
			default:
				GAME_THROW_MSG ("Unsupported precision");
		}
	}

	virtual void Prepare (BufferResource& _buffer) override final
	{
		GAME_ASSERT_MSG (pProvider, "Buffer provider is nullptr");
		_buffer.bInitialize = true;
		// Description
		_buffer.description.BindFlags = D3D11_BIND_INDEX_BUFFER;
		_buffer.description.ByteWidth = static_cast<UINT>(pProvider->GetLength () * sizeof (ind_t));
		_buffer.description.CPUAccessFlags = 0;
		_buffer.description.MiscFlags = 0;
		_buffer.description.StructureByteStride = static_cast<UINT>(sizeof (ind_t));
		_buffer.description.Usage = D3D11_USAGE_DEFAULT;
		// Data
		_buffer.data.pSysMem = pProvider->GetData ();
		_buffer.data.SysMemPitch = 0;
		_buffer.data.SysMemSlicePitch = 0;
	}

};

class VertexBufferResourceBase : public IABufferResourceBase
{

public:

	static void Set (ID3D11DeviceContext& deviceContext, int startingSlot, const std::vector<const VertexBufferResourceBase *> buffers);

	void Set (ID3D11DeviceContext& deviceContext, int slot) const;

protected:

	virtual size_t GetStride () const = 0;

};

template<typename T>
class StaticVertexBufferResource : public VertexBufferResourceBase
{

public:

	int GetLength () const
	{
		GAME_ASSERT_MSG (pProvider, "Buffer provider is nullptr");
		return pProvider->GetLength ();
	}

	const BufferProvider<T> * pProvider;

private:

	virtual void Prepare (BufferResource & _buffer) override
	{
		GAME_ASSERT_MSG (pProvider, "Buffer provider is nullptr");
		_buffer.bInitialize = true;
		// Description
		_buffer.description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		_buffer.description.ByteWidth = static_cast<UINT>(pProvider->GetLength () * sizeof (ind_t));
		_buffer.description.CPUAccessFlags = 0;
		_buffer.description.MiscFlags = 0;
		_buffer.description.StructureByteStride = static_cast<UINT>(sizeof (ind_t));
		_buffer.description.Usage = D3D11_USAGE_DEFAULT;
		// Data
		_buffer.data.pSysMem = pProvider->GetData ();
		_buffer.data.SysMemPitch = 0;
		_buffer.data.SysMemSlicePitch = 0;
	}

	virtual size_t GetStride () const override
	{
		return sizeof (T);
	}

};