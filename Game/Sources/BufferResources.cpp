#include <Game\Resources\BufferResources.hpp>
#include "Include\Game\Resources\BufferResources.hpp"

BufferResource::BufferResource (BindMode _bindModes, bool _bImmutable, bool _bReadable, size_t _structSize, int _length)
	: m_BindModes { _bindModes }, m_bImmutable { _bImmutable }, m_bReadable { _bReadable }, m_StructSize { static_cast<UINT>(_structSize) }, m_Length { _length }
{
	GAME_ASSERT_MSG (!_bReadable || _bindModes == BindMode::None, "Readable resources cannot be bound");
	GAME_ASSERT_MSG (_length > 0, "Lenght must be positive");
	GAME_ASSERT_MSG (_structSize > 0, "Struct size must be positive");
	GAME_ASSERT_MSG (!(_bindModes & BindMode::ConstantBuffer) || (_structSize * _length) % 16 == 0, "Constant buffer size must be multiple of 16");
}

void BufferResource::Update (ID3D11DeviceContext& _deviceContext, const void * _pData, size_t _cData, int _destOffset)
{
	GAME_ASSERT_MSG (!m_bImmutable, "Immutable resource");
	GAME_ASSERT_MSG (_cData > 0 && _destOffset > 0, "Invalid data size arguments");
	ID3D11Buffer * pBuffer { GetPointer () };
	if (m_BindModes & BindMode::StreamOutput)
	{
		D3D11_BOX box;
		box.left = static_cast<UINT>(_destOffset);
		box.right = static_cast<UINT>(_destOffset + _cData);
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;
		_deviceContext.UpdateSubresource (pBuffer, 0, &box, _pData, 0, 0);
	}
	else
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource {};
		GAME_COMC (_deviceContext.Map (pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		memcpy (reinterpret_cast<BYTE*>(mappedResource.pData) + _destOffset, _pData, _cData);
		GAME_COMC (_deviceContext.Unmap (pBuffer, 0));
	}
}

void BufferResource::Retrieve (void * _pData, int _cData) const
{
	UNREFERENCED_PARAMETER (_pData);
	UNREFERENCED_PARAMETER (_cData);
	GAME_ASSERT_MSG (m_bReadable, "Not a readable resource");
	GAME_THROW_MSG ("Unimplemented");
	// TODO implement
}

void BufferResource::CopyTo (BufferResource & _resource) const
{
	GAME_ASSERT_MSG (!m_bImmutable || _resource.m_BindModes & BindMode::StreamOutput, "Immutable resource without StreamOutput bind mode");
	GAME_THROW_MSG ("Unimplemented");
	// TODO implement
}

BufferResource::BindMode BufferResource::GetBindModes () const
{
	return m_BindModes;
}

bool BufferResource::IsImmutable () const
{
	return m_bImmutable;
}

bool BufferResource::IsReadable () const
{
	return m_bReadable;
}

size_t BufferResource::GetStructSize () const
{
	return static_cast<size_t>(m_StructSize);
}

int BufferResource::GetLength () const
{
	return m_Length;
}

void BufferResource::ResetIndexBuffer (ID3D11DeviceContext & _deviceContext)
{
	_deviceContext.IASetIndexBuffer (nullptr, DXGI_FORMAT_UNKNOWN, 0);
}

void BufferResource::SetVertexBuffers (ID3D11DeviceContext & _deviceContext, UINT _startingSlot, const BufferResource*const* _pBuffers, const UINT * _pStructSizes, const UINT * _pOffsets, int _cBuffer)
{
	GAME_ASSERT_MSG (_cBuffer >= 0, "Buffer count cannot be negative");
	if (_cBuffer > 0)
	{
		std::vector<ID3D11Buffer*> bufs (static_cast<size_t>(_cBuffer));
		for (int iBuf { 0 }; iBuf < _cBuffer; iBuf++)
		{
			if (_pBuffers[iBuf])
			{
				GAME_ASSERT_MSG (_pBuffers[iBuf]->IsCreated (), "Not created");
				GAME_ASSERT_MSG (_pBuffers[iBuf]->m_BindModes & BindMode::VertexBuffer, "Not a vertex buffer");
				bufs[iBuf] = _pBuffers[iBuf]->GetPointer ();
			}
			else
			{
				bufs[iBuf] = nullptr;
			}
		}
		_deviceContext.IASetVertexBuffers (_startingSlot, static_cast<UINT>(_cBuffer), bufs.data (), _pStructSizes, _pOffsets);
	}
}

void BufferResource::SetVertexBuffers (ID3D11DeviceContext & _deviceContext, UINT _startingSlot, const BufferResource*const* _pBuffers, int _cBuffer)
{
	GAME_ASSERT_MSG (_cBuffer >= 0, "Buffer count cannot be negative");
	if (_cBuffer > 0)
	{
		std::vector<UINT> structSizes (static_cast<size_t>(_cBuffer));
		for (int iBuf { 0 }; iBuf < _cBuffer; iBuf++)
		{
			if (_pBuffers[iBuf])
			{
				GAME_ASSERT_MSG (_pBuffers[iBuf]->IsCreated (), "Not created");
				GAME_ASSERT_MSG (_pBuffers[iBuf]->m_BindModes & BindMode::VertexBuffer, "Not a vertex buffer");
				structSizes[iBuf] = _pBuffers[iBuf]->m_StructSize;
			}
			else
			{
				structSizes[iBuf] = 0;
			}
		}
		std::vector<UINT> offsets (static_cast<size_t>(_cBuffer), 0);
		SetVertexBuffers (_deviceContext, _startingSlot, _pBuffers, structSizes.data (), offsets.data (), _cBuffer);
	}
}

void BufferResource::SetConstantBuffers (ID3D11DeviceContext & _deviceContext, UINT _startingSlot, ShaderType _shader, const BufferResource*const* _pBuffers, int _cBuffer)
{
	GAME_ASSERT_MSG (_cBuffer >= 0, "Buffer count cannot be negative");
	if (_cBuffer > 0)
	{
		std::vector<ID3D11Buffer*> bufs (static_cast<size_t>(_cBuffer));
		for (int iBuf { 0 }; iBuf < _cBuffer; iBuf++)
		{
			if (_pBuffers[iBuf])
			{
				GAME_ASSERT_MSG (_pBuffers[iBuf]->IsCreated (), "Not created");
				GAME_ASSERT_MSG (_pBuffers[iBuf]->m_BindModes & BindMode::VertexBuffer, "Not a vertex buffer");
				bufs[iBuf] = _pBuffers[iBuf]->GetPointer ();
			}
			else
			{
				bufs[iBuf] = nullptr;
			}
		}
		SetConstantBuffers (_deviceContext, static_cast<UINT>(_startingSlot), static_cast<UINT>(_cBuffer), bufs.data (), _shader);
	}
}

void BufferResource::SetStreamOutputBuffers (ID3D11DeviceContext & _deviceContext, const BufferResource*const* _pBuffers, const UINT * _pOffsets, int _cBuffer)
{
	GAME_ASSERT_MSG (_cBuffer >= 0, "Buffer count cannot be negative");
	if (_cBuffer > 0)
	{
		std::vector<ID3D11Buffer*> bufs (static_cast<size_t>(_cBuffer));
		for (int iBuf { 0 }; iBuf < _cBuffer; iBuf++)
		{
			if (_pBuffers[iBuf])
			{
				GAME_ASSERT_MSG (_pBuffers[iBuf]->IsCreated (), "Not created");
				GAME_ASSERT_MSG (_pBuffers[iBuf]->m_BindModes & BindMode::VertexBuffer, "Not a vertex buffer");
				bufs[iBuf] = _pBuffers[iBuf]->GetPointer ();
			}
			else
			{
				bufs[iBuf] = nullptr;
			}
		}
		_deviceContext.SOSetTargets (static_cast<UINT>(_cBuffer), bufs.data (), _pOffsets);
	}
}

void BufferResource::SetStreamOutputBuffers (ID3D11DeviceContext & _deviceContext, const BufferResource*const* _pBuffers, int _cBuffer)
{
	std::vector<UINT> offsets (static_cast<size_t>(_cBuffer), 0);
	SetStreamOutputBuffers (_deviceContext, _pBuffers, offsets.data (), _cBuffer);
}

void BufferResource::SetIndexBuffer (ID3D11DeviceContext & _deviceContext, size_t _structSize, UINT _offset) const
{
	DXGI_FORMAT format;
	switch (_structSize)
	{
		case 1:
			format = DXGI_FORMAT_R8_UINT;
			break;
		case 2:
			format = DXGI_FORMAT_R16_UINT;
			break;
		case 4:
			format = DXGI_FORMAT_R32_UINT;
			break;
		default:
			GAME_THROW_MSG ("Unsupported index size");
	}
	_deviceContext.IASetIndexBuffer (GetPointer (), format, _offset);
}

void BufferResource::SetIndexBuffer (ID3D11DeviceContext & _deviceContext) const
{
	SetIndexBuffer (_deviceContext, m_StructSize, 0);
}

void BufferResource::SetVertexBuffer (ID3D11DeviceContext & _deviceContext, UINT _slot, UINT _structSize, UINT _offset) const
{
	ID3D11Buffer* pBuf { GetPointer () };
	_deviceContext.IASetVertexBuffers (_slot, 1, &pBuf, &_structSize, &_offset);
}

void BufferResource::SetVertexBuffer (ID3D11DeviceContext & _deviceContext, UINT _slot) const
{
	SetVertexBuffer (_deviceContext, _slot, m_StructSize, 0);
}

void BufferResource::SetConstantBuffer (ID3D11DeviceContext & _deviceContext, UINT _slot, ShaderType _shader) const
{
	ID3D11Buffer* pBuf { GetPointer () };
	SetConstantBuffers (_deviceContext, _slot, 1, &pBuf, _shader);
}

void BufferResource::SetStreamOutputBuffer (ID3D11DeviceContext & _deviceContext, UINT _offset) const
{
	ID3D11Buffer* pBuf { GetPointer () };
	_deviceContext.SOSetTargets (1, &pBuf, &_offset);
}

void BufferResource::SetStreamOutputBuffer (ID3D11DeviceContext & _deviceContext) const
{
	SetStreamOutputBuffer (_deviceContext, 0);
}

ID3D11Buffer * BufferResource::Create (ID3D11Device & _device) const
{
	D3D11_BUFFER_DESC description;

	description.ByteWidth = static_cast<UINT>(m_Length * m_StructSize);

	description.BindFlags = 0;
	if (m_BindModes & BindMode::IndexBuffer)
	{
		description.BindFlags |= D3D11_BIND_INDEX_BUFFER;
	}
	if (m_BindModes & BindMode::VertexBuffer)
	{
		description.BindFlags |= D3D11_BIND_VERTEX_BUFFER;
	}
	if (m_BindModes & BindMode::ConstantBuffer)
	{
		description.BindFlags |= D3D11_BIND_CONSTANT_BUFFER;
	}
	if (m_BindModes & BindMode::StreamOutput)
	{
		description.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
	}

	description.CPUAccessFlags = 0;
	if (m_bReadable)
	{
		description.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
	}
	if (!m_bImmutable)
	{
		description.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	}

	description.MiscFlags = 0;

	description.StructureByteStride = m_StructSize;

	if (m_bReadable)
	{
		description.Usage = D3D11_USAGE_STAGING;
	}
	else
	{
		if (m_BindModes & BindMode::StreamOutput)
		{
			description.Usage = D3D11_USAGE_DEFAULT;
		}
		else
		{
			description.Usage = m_bImmutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
		}
	}

	ID3D11Buffer * pBuffer;

	const void * pInitialData { ProvideInitialData () };

	if (pInitialData)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = pInitialData;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		GAME_COMC (_device.CreateBuffer (&description, &data, &pBuffer));
	}
	else
	{
		GAME_ASSERT_MSG (!m_bImmutable || m_BindModes & BindMode::StreamOutput, "Immutable resources without StreamOutput bind mode must provide initial data");
		GAME_COMC (_device.CreateBuffer (&description, nullptr, &pBuffer));
	}

	return pBuffer;
}

void BufferResource::SetConstantBuffers (ID3D11DeviceContext & _deviceContext, UINT _startingSlot, UINT _count, ID3D11Buffer * const * _pBuffers, ShaderType _shader)
{
	switch (_shader)
	{
		case ShaderType::VertexShader:
			_deviceContext.VSSetConstantBuffers (_startingSlot, _count, _pBuffers);
			break;
		case ShaderType::PixelShader:
			_deviceContext.PSSetConstantBuffers (_startingSlot, _count, _pBuffers);
			break;
		case ShaderType::GeometryShader:
			_deviceContext.GSSetConstantBuffers (_startingSlot, _count, _pBuffers);
			break;
		case ShaderType::HullShader:
			_deviceContext.HSSetConstantBuffers (_startingSlot, _count, _pBuffers);
			break;
		case ShaderType::DomainShader:
			_deviceContext.DSSetConstantBuffers (_startingSlot, _count, _pBuffers);
			break;
		case ShaderType::ComputeShader:
			_deviceContext.CSSetConstantBuffers (_startingSlot, _count, _pBuffers);
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
	}
}

const void * GenericBufferResource::ProvideInitialData () const
{
	return pInitialData;
}

IndexBufferResourceBase::IndexBufferResourceBase (bool _bImmutable, size_t _indSize, int _length)
	: BufferResource { BindMode::IndexBuffer, _bImmutable, false, _indSize, _length }
{
	switch (_indSize)
	{
		case 1:
		case 2:
		case 4:
			break;
		default:
			GAME_THROW_MSG ("Unsupported index size");
	}
}

void IndexBufferResourceBase::Reset (ID3D11DeviceContext & _deviceContext)
{
	BufferResource::ResetIndexBuffer (_deviceContext);
}

void IndexBufferResourceBase::Set (ID3D11DeviceContext & _deviceContext) const
{
	BufferResource::SetIndexBuffer (_deviceContext);
}

VertexBufferResourceBase::VertexBufferResourceBase (bool _bImmutable, size_t _vertSize, int _length)
	: BufferResource { BindMode::VertexBuffer, _bImmutable, false, _vertSize, _length }
{}

void VertexBufferResourceBase::Set (ID3D11DeviceContext & _deviceContext, UINT _startingSlot, const VertexBufferResourceBase * const * _pBuffers, int _cBuffers)
{
	BufferResource::SetVertexBuffers (_deviceContext, _startingSlot, reinterpret_cast<const BufferResource * const *>(_pBuffers), _cBuffers);
}

void VertexBufferResourceBase::Set (ID3D11DeviceContext & _deviceContext, UINT _slot) const
{
	BufferResource::SetVertexBuffer (_deviceContext, _slot);
}

ConstantBufferResourceBase::ConstantBufferResourceBase (bool _bImmutable, size_t _size)
	: BufferResource { BindMode::ConstantBuffer, _bImmutable, false, _size, 1 }
{}

void ConstantBufferResourceBase::Set (ID3D11DeviceContext & _deviceContext, UINT _startingSlot, ShaderType _shader, const ConstantBufferResourceBase * const * _pBuffers, int _cBuffers)
{
	BufferResource::SetConstantBuffers (_deviceContext, _startingSlot, _shader, reinterpret_cast<const BufferResource * const *>(_pBuffers), _cBuffers);
}

void ConstantBufferResourceBase::Set (ID3D11DeviceContext & _deviceContext, UINT _slot, ShaderType _shader) const
{
	BufferResource::SetConstantBuffer (_deviceContext, _slot, _shader);
}