#include "Include\Game\Resources\BufferResources.hpp"

void BufferResource::ForceCreate (ID3D11Device & _device)
{
	GAME_ASSERT_MSG (!IsCreated (), "Already created");
	D3D11_BUFFER_DESC desc;
	switch (GetType ())
	{
		case Type::IndexBuffer:
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			break;
		case Type::VertexBuffer:
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
	}
	desc.ByteWidth = static_cast<UINT>(GetLength () * GetStride ());
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = static_cast<UINT>(GetStride ());
	desc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = GetData ();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	GAME_COMC (_device.CreateBuffer (&desc, &data, &m_pBuffer));
}

void BufferResource::ForceDestroy ()
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	m_pBuffer->Release ();
	m_pBuffer = nullptr;
}

bool BufferResource::IsCreated () const
{
	return m_pBuffer;
}

ID3D11Buffer * BufferResource::GetBuffer () const
{
	return m_pBuffer;
}

void VertexBufferResource::Set (ID3D11DeviceContext & _deviceContext, int _startingSlot, const std::vector<const VertexBufferResource*>& _buffers)
{
	if (!_buffers.empty ())
	{
		std::vector<ID3D11Buffer*> bufs { _buffers.size () };
		std::vector<UINT> offsets (_buffers.size ());
		std::vector<UINT> strides (_buffers.size ());
		for (int iBuf { 0 }; iBuf < _buffers.size (); iBuf++)
		{
			if (_buffers[iBuf])
			{
				GAME_ASSERT_MSG (_buffers[iBuf]->IsCreated (), "Not created");
				bufs[iBuf] = _buffers[iBuf]->GetBuffer ();
				offsets[iBuf] = 0;
				strides[iBuf] = static_cast<UINT>(_buffers[iBuf]->GetStride ());
			}
			else
			{
				bufs[iBuf] = nullptr;
				offsets[iBuf] = 0;
				strides[iBuf] = 0;
			}
		}
		GAME_COMC (_deviceContext.IASetVertexBuffers (static_cast<UINT>(_startingSlot), 1, bufs.data (), strides.data (), offsets.data ()));
	}
}

void VertexBufferResource::Set (ID3D11DeviceContext & _deviceContext, int _slot)
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	UINT offsets[] { 0 };
	UINT strides[] { static_cast<UINT>(GetStride ()) };
	ID3D11Buffer * pBuffers[] { GetBuffer () };
	GAME_COMC (_deviceContext.IASetVertexBuffers (static_cast<UINT>(_slot), 1, pBuffers, strides, offsets));
}

void IndexBufferResource::Reset (ID3D11DeviceContext & _deviceContext)
{
	GAME_COMC (_deviceContext.IASetIndexBuffer (nullptr, DXGI_FORMAT_UNKNOWN, 0));
}

void IndexBufferResource::Set (ID3D11DeviceContext& _deviceContext) const
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	DXGI_FORMAT format;
	switch (GetFormat ())
	{
		case Format::R16UINT:
			format = DXGI_FORMAT_R16_UINT;
			break;
		case Format::R32UINT:
			format = DXGI_FORMAT_R32_UINT;
			break;
		default:
			GAME_THROW_MSG ("Unknown format");
	}
	GAME_COMC (_deviceContext.IASetIndexBuffer (GetBuffer (), format, 0));
}