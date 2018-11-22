#include <Game/Resources/ConstantBufferResource.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>
#include <vector>

void ConstantBufferResource::Set (ID3D11DeviceContext & _deviceContext, int _startingSlot, const std::vector<const ConstantBufferResource*>& _buffers, ShaderType _shaderType)
{
	if (!_buffers.empty ())
	{
		std::vector<ID3D11Buffer*> bufs { _buffers.size () };
		for (int iBuf { 0 }; iBuf < _buffers.size (); iBuf++)
		{
			if (_buffers[iBuf])
			{
				GAME_ASSERT_MSG (_buffers[iBuf]->IsCreated (), "Not created");
				bufs[iBuf] = _buffers[iBuf]->m_pBuffer;
			}
			else
			{
				bufs[iBuf] = nullptr;
			}
		}
		switch (_shaderType)
		{
			case ShaderType::VertexShader:
				_deviceContext.VSSetConstantBuffers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_buffers.size ()), bufs.data ());
				break;
			case ShaderType::PixelShader:
				_deviceContext.PSSetConstantBuffers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_buffers.size ()), bufs.data ());
				break;
			case ShaderType::GeometryShader:
				_deviceContext.GSSetConstantBuffers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_buffers.size ()), bufs.data ());
				break;
			case ShaderType::HullShader:
				_deviceContext.HSSetConstantBuffers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_buffers.size ()), bufs.data ());
				break;
			case ShaderType::DomainShader:
				_deviceContext.DSSetConstantBuffers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_buffers.size ()), bufs.data ());
				break;
			case ShaderType::ComputeShader:
				_deviceContext.CSSetConstantBuffers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_buffers.size ()), bufs.data ());
				break;
			default:
				GAME_THROW_MSG ("Unknown type");
				break;
		}
	}
}

ConstantBufferResource::ConstantBufferResource (int _size) : m_cBuffer { static_cast<UINT>(_size) }
{
	GAME_ASSERT_MSG (_size % 16 == 0, "Size is not a multiple of 16");
	GAME_ASSERT_MSG (_size / 16 <= D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT, "Size exceeds maximum value");
}

void ConstantBufferResource::Update (ID3D11DeviceContext & _deviceContext, const void * _pData, int _cData) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource {};
	_deviceContext.Map (m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy (mappedResource.pData, _pData, static_cast<size_t>(_cData));
	_deviceContext.Unmap (m_pBuffer, 0);
}

void ConstantBufferResource::Set (ID3D11DeviceContext & _deviceContext, int _slot, ShaderType _shaderType) const
{
	switch (_shaderType)
	{
		case ShaderType::VertexShader:
			_deviceContext.VSSetConstantBuffers (static_cast<UINT>(_slot), 1, &m_pBuffer);
			break;
		case ShaderType::PixelShader:
			_deviceContext.PSSetConstantBuffers (static_cast<UINT>(_slot), 1, &m_pBuffer);
			break;
		case ShaderType::GeometryShader:
			_deviceContext.GSSetConstantBuffers (static_cast<UINT>(_slot), 1, &m_pBuffer);
			break;
		case ShaderType::HullShader:
			_deviceContext.HSSetConstantBuffers (static_cast<UINT>(_slot), 1, &m_pBuffer);
			break;
		case ShaderType::DomainShader:
			_deviceContext.DSSetConstantBuffers (static_cast<UINT>(_slot), 1, &m_pBuffer);
			break;
		case ShaderType::ComputeShader:
			_deviceContext.CSSetConstantBuffers (static_cast<UINT>(_slot), 1, &m_pBuffer);
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
			break;
	}
}

void ConstantBufferResource::ForceCreate (ID3D11Device & _device)
{
	GAME_ASSERT_MSG (!ConstantBufferResource::IsCreated (), "Already created");
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = m_cBuffer;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	GAME_COMC (_device.CreateBuffer (&desc, nullptr, &m_pBuffer));
}

void ConstantBufferResource::ForceDestroy ()
{
	GAME_ASSERT_MSG (ConstantBufferResource::IsCreated (), "Not created");
	m_pBuffer->Release ();
	m_pBuffer = nullptr;
}

bool ConstantBufferResource::IsCreated () const
{
	return m_pBuffer != nullptr;
}

int ConstantBufferResource::GetSize () const
{
	return m_cBuffer;
}