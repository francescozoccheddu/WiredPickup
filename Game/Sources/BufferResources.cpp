#include <Game\Resources\BufferResources.hpp>

ID3D11Buffer * BufferResource::Create (ID3D11Device & _device) const
{
	ID3D11Buffer * pBuffer;
	GAME_COMC (_device.CreateBuffer (&description, bInitialize ? &data : nullptr, &pBuffer));
	return pBuffer;
}

void IABufferResourceBase::ForceCreate (ID3D11Device & _device)
{
	GAME_RESOURCE_ASSERT_NOTCREATED;
	Prepare (m_Buffer);
	m_Buffer.ForceCreate (_device);
}

void IABufferResourceBase::ForceDestroy ()
{
	m_Buffer.ForceDestroy ();
}

bool IABufferResourceBase::IsCreated () const
{
	return m_Buffer.IsCreated ();
}

ID3D11Buffer * IABufferResourceBase::GetPointer () const
{
	return m_Buffer.GetPointer ();
}

void IndexBufferResourceBase::Reset (ID3D11DeviceContext & _deviceContext)
{
	_deviceContext.IASetIndexBuffer (nullptr, DXGI_FORMAT_UNKNOWN, 0);
}

void IndexBufferResourceBase::Set (ID3D11DeviceContext & _deviceContext) const
{
	GAME_RESOURCE_ASSERT_CREATED;
	_deviceContext.IASetIndexBuffer (GetPointer (), GetFormat (), 0);
}

void VertexBufferResourceBase::Set (ID3D11DeviceContext & _deviceContext, int _startingSlot, const std::vector<const VertexBufferResourceBase*> _buffers)
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
				bufs[iBuf] = _buffers[iBuf]->GetPointer ();
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
		_deviceContext.IASetVertexBuffers (static_cast<UINT>(_startingSlot), 1, bufs.data (), strides.data (), offsets.data ());
	}
}

void VertexBufferResourceBase::Set (ID3D11DeviceContext & _deviceContext, int _slot) const
{
	GAME_RESOURCE_ASSERT_CREATED;
	const UINT strides[] { static_cast<UINT>(GetStride ()) };
	const UINT offsets[] { 0 };
	ID3D11Buffer * pBuffers[] { GetPointer () };
	_deviceContext.IASetVertexBuffers (static_cast<UINT>(_slot), 1, pBuffers, strides, offsets);
}
