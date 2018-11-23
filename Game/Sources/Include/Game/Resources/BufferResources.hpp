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

	void Update (ID3D11DeviceContext& deviceContext, const void * pData, int cData, int destOffset);

	void Retrieve (void * pData, int cData) const;

	void CopyTo (BufferResource& resource) const;

	BindMode GetBindModes () const;

	bool IsImmutable () const;

	bool IsReadable () const;

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

	static constexpr inline int GetBoundingConstantBufferSize (int _size)
	{
		int remainder = _size % 16;
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

/*

class IndexBufferResource : public BufferResource
{

public:

	//TODO Add constructors

	static void Reset (ID3D11DeviceContext& deviceContext);

	void Set (ID3D11DeviceContext& deviceContext) const;

};

template<unsigned int precBytes>
class VectorIndexBufferResource : public IndexBufferResource
{

	static_assert(precBytes == 1 || precBytes == 2 || precBytes == 4, "Unsupported precision");

public:

	//TODO Add constructors

	std::vector<ind_t> data;
	bool bInitialize { false };

	using ind_t = std::conditional<precBytes == 1, uint8_t, std::conditional<precBytes == 2, uint16_t, uint32_t>>;

	void Update (ID3D11DeviceContext& _deviceContext, int _count)
	{
		GAME_ASSERT_MSG (_srcOffset >= 0 && _count >= 0, "Count and offset arguments cannot be negative");
		GAME_ASSERT_MSG (_srcOffset + _count > data.size (), "Out of bounds");
		BufferResource::Update (_deviceContext, data.data (), static_cast<int>(_count * sizeof (ind_t)), 0);
	}

	void Update (ID3D11DeviceContext& _deviceContext)
	{
		Update (_deviceContext, data.size ());
	}

};

class VertexBufferResource : public BufferResource
{

public:

	//TODO Add constructors

	static void Set (ID3D11DeviceContext & deviceContext, int startingSlot, const std::vector<const BufferResource*>& buffers);

	void Set (ID3D11DeviceContext& deviceContext) const;

};

template<typename T>
class VectorVertexBufferResource : public VertexBufferResource
{

public:

	//TODO Add constructors

	std::vector<T> data;
	bool bInitialize { false };

	void Update (ID3D11DeviceContext& _deviceContext, int _count)
	{
		GAME_ASSERT_MSG (_srcOffset >= 0 && _count >= 0, "Count and offset arguments cannot be negative");
		GAME_ASSERT_MSG (_srcOffset + _count > data.size (), "Out of bounds");
		BufferResource::Update (_deviceContext, data.data (), static_cast<int>(_count * sizeof (T)), 0);
	}

	void Update (ID3D11DeviceContext& _deviceContext)
	{
		Update (_deviceContext, data.size ());
	}

private:


};

class ConstantBufferResource : public BufferResource
{

public:

	//TODO Add constructors

	static void Set (ID3D11DeviceContext & deviceContext, int startingSlot, const std::vector<const BufferResource*>& buffers, ShaderType shaderType);

	void Set (ID3D11DeviceContext & deviceContext, int slot, ShaderType shaderType) const;

	static constexpr int GetBoundingBufferSize (int _size)
	{
		int remainder = _size % 16;
		if (remainder > 0)
		{
			return _size + 16 - remainder;
		}
		else
		{
			return _size;
		}
	}

};

template<typename T>
class StructConstantBufferResource final : public ConstantBufferResource
{

public:

	//TODO Add constructors

	T data;
	bool bInitialize { false };

	void Update (ID3D11DeviceContext& _deviceContext, int _size)
	{
		GAME_ASSERT_MSG (_size > 0 && _size < sizeof (T), "Invalid size");
		BufferResource::Update (_deviceContext, &data, _size, 0);
	}

	void Update (ID3D11DeviceContext& _deviceContext)
	{
		Update (_deviceContext, static_cast<int>(sizeof (T)));
	}

private:


};

*/