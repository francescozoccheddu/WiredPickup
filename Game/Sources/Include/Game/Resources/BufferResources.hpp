#pragma once

#include <Game/Resources/Resource.hpp>
#include <Game/Resources/ShaderType.hpp>
#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>
#include <vector>
#include <cstdint>

class BufferResource : public SimpleResource<ID3D11Buffer>
{

public:

	enum BindMode
	{
		None = 0,
		IndexBuffer = 1 << 0,
		VertexBuffer = 1 << 1,
		ConstantBuffer = 1 << 2,
		StreamOutput = 1 << 3
	};


	BufferResource (BindMode bindModes, bool bImmutable, bool bReadable, size_t structSize, int length);

	void Update (ID3D11DeviceContext& deviceContext, const void * pData, size_t cData, int destOffset);

	void Retrieve (void * pData, int cData) const;

	void CopyTo (BufferResource& resource) const;

	BindMode GetBindModes () const;

	bool IsImmutable () const;

	bool IsReadable () const;

	size_t GetStructSize () const;

	int GetLength () const;

	// TODO check slot count (like D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT)

	static void ResetIndexBuffer (ID3D11DeviceContext& deviceContext);

	static void SetVertexBuffers (ID3D11DeviceContext& deviceContext, UINT startingSlot, const BufferResource*const* pBuffers, const UINT * pStructSizes, const UINT * pOffsets, int cBuffers);

	static void SetVertexBuffers (ID3D11DeviceContext& deviceContext, UINT startingSlot, const BufferResource*const* pBuffers, int cBuffers);

	static void SetConstantBuffers (ID3D11DeviceContext& deviceContext, UINT startingSlot, ShaderType shader, const BufferResource*const* pBuffers, int cBuffers);

	static void SetStreamOutputBuffers (ID3D11DeviceContext& deviceContext, const BufferResource*const* pBuffers, const UINT * pOffsets, int cBuffers);

	static void SetStreamOutputBuffers (ID3D11DeviceContext& deviceContext, const BufferResource*const* pBuffers, int cBuffers);

	void SetIndexBuffer (ID3D11DeviceContext& deviceContext, size_t structSize, UINT offset) const;

	void SetIndexBuffer (ID3D11DeviceContext& deviceContext) const;

	void SetVertexBuffer (ID3D11DeviceContext& deviceContext, UINT slot, UINT structSize, UINT offset) const;

	void SetVertexBuffer (ID3D11DeviceContext& deviceContext, UINT slot) const;

	void SetConstantBuffer (ID3D11DeviceContext& deviceContext, UINT slot, ShaderType shader) const;

	void SetStreamOutputBuffer (ID3D11DeviceContext& deviceContext, UINT offset) const;

	void SetStreamOutputBuffer (ID3D11DeviceContext& deviceContext) const;

	static constexpr inline size_t GetBoundingConstantBufferSize (size_t _size)
	{
		size_t remainder = _size % 16;
		if (remainder > 0)
		{
			return _size + 16 - remainder;
		}
		else
		{
			return _size;
		}
	}

protected:

	virtual ID3D11Buffer * Create (ID3D11Device& device) const override final;

	virtual const void * ProvideInitialData () const = 0;

private:

	const BindMode m_BindModes;
	const bool m_bImmutable;
	const bool m_bReadable;
	const UINT m_StructSize;
	const int m_Length;

	static void SetConstantBuffers (ID3D11DeviceContext & _deviceContext, UINT _startingSlot, UINT _count, ID3D11Buffer * const * _pBuffers, ShaderType _shader);

};

inline BufferResource::BindMode operator | (BufferResource::BindMode a, BufferResource::BindMode b)
{
	return static_cast<BufferResource::BindMode>(static_cast<int>(a) | static_cast<int>(b));
}

inline BufferResource::BindMode operator & (BufferResource::BindMode a, BufferResource::BindMode b)
{
	return static_cast<BufferResource::BindMode>(static_cast<int>(a) & static_cast<int>(b));
}

inline BufferResource::BindMode& operator |= (BufferResource::BindMode& a, BufferResource::BindMode b)
{
	return a = a | b;
}

class GenericBufferResource : public BufferResource
{

public:

	using BufferResource::BufferResource;

	const void * pInitialData { nullptr };

private:

	virtual const void * ProvideInitialData () const override final;

};

class IndexBufferResourceBase : public BufferResource
{

public:

	IndexBufferResourceBase (bool bImmutable, size_t vertSize, int length);

	static void Reset (ID3D11DeviceContext& deviceContext);

	void Set (ID3D11DeviceContext& deviceContext) const;

};

class VertexBufferResourceBase : public BufferResource
{

public:

	VertexBufferResourceBase (bool bImmutable, size_t vertSize, int length);

	static void Set (ID3D11DeviceContext& deviceContext, UINT startingSlot, const VertexBufferResourceBase*const* pBuffers, int cBuffers);

	void Set (ID3D11DeviceContext& deviceContext, UINT slot) const;

};

class ConstantBufferResourceBase : public BufferResource
{

public:

	ConstantBufferResourceBase (bool bImmutable, size_t size);

	static void Set (ID3D11DeviceContext& deviceContext, UINT startingSlot, ShaderType shader, const ConstantBufferResourceBase*const* pBuffers, int cBuffers);

	void Set (ID3D11DeviceContext& deviceContext, UINT slot, ShaderType shader) const;

};


template<int indSize, bool bInitialize, int length>
class SimpleIndexBufferResource : public IndexBufferResourceBase
{

	static_assert(indSize == 1 || indSize == 2 || indSize == 4, "Unsupported index size");

public:

	using ind_t = std::conditional<indSize == 1, uint8_t, std::conditional<indSize == 2, uint16_t, uint32_t>>;

	SimpleIndexBufferResource (bool _bImmutable) : IndexBufferResourceBase { _bImmutable, sizeof (ind_t), length } {}

	SimpleIndexBufferResource (bool _bImmutable, const std::array<ind_t, static_cast<size_t>(length)>& _indices) : IndexBufferResourceBase { _bImmutable, sizeof (ind_t), length }, indices { _indices } {}

	SimpleIndexBufferResource (bool _bImmutable, const std::array<ind_t, static_cast<size_t>(length)>&& _indices) : IndexBufferResourceBase { _bImmutable, sizeof (ind_t), length }, indices { _indices } {}

	std::array<ind_t, static_cast<size_t>(length)> indices;

	void Update (ID3D11DeviceContext& _deviceContext)
	{
		BufferResource::Update (_deviceContext, indices.data (), static_cast<size_t>(length * sizeof (ind_t)), 0);
	}

private:

	virtual const void * ProvideInitialData () const override
	{
		return bInitialize ? indices.data () : nullptr;
	}

};

template<typename T, bool bInitialize, int length>
class SimpleVertexBufferResource : public VertexBufferResourceBase
{

public:

	SimpleVertexBufferResource (bool _bImmutable) : VertexBufferResourceBase { _bImmutable, sizeof (T), length } {}

	SimpleVertexBufferResource (bool _bImmutable, const std::array<T, static_cast<size_t>(length)>& _vertices) : VertexBufferResourceBase { _bImmutable, sizeof (ind_t), length }, vertices { _vertices } {}

	SimpleVertexBufferResource (bool _bImmutable, const std::array<T, static_cast<size_t>(length)>&& _vertices) : VertexBufferResourceBase { _bImmutable, sizeof (ind_t), length }, vertices { _vertices } {}

	std::array<T, static_cast<size_t>(length)> vertices;

	void Update (ID3D11DeviceContext& _deviceContext)
	{
		BufferResource::Update (_deviceContext, vertices.data (), static_cast<size_t>(length * sizeof (T)), 0);
	}

private:

	virtual const void * ProvideInitialData () const override
	{
		return bInitialize ? vertices.data () : nullptr;
	}

};

template<typename T, bool bInitialize>
class SimpleConstantBufferResource : public ConstantBufferResourceBase
{

public:

	SimpleConstantBufferResource (bool _bImmutable) : ConstantBufferResourceBase { _bImmutable, GetBoundingConstantBufferSize (sizeof (T)) } {}

	SimpleConstantBufferResource (bool _bImmutable, const T& _data) : ConstantBufferResourceBase { _bImmutable, sizeof (ind_t) }, data { _data } {}

	SimpleConstantBufferResource (bool _bImmutable, const T&& _data) : ConstantBufferResourceBase { _bImmutable, sizeof (ind_t) }, data { _data } {}

	T data;

	void Update (ID3D11DeviceContext& _deviceContext)
	{
		BufferResource::Update (_deviceContext, &data, sizeof (T), 0);
	}

private:

	virtual const void * ProvideInitialData () const override
	{
		return bInitialize ? &data : nullptr;
	}

};