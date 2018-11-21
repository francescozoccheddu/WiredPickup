#include <Game/Resources/MeshResource.hpp>

#include <Game/Utils/Storage.hpp>
#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

MeshResource::~MeshResource ()
{
	if (MeshResource::IsCreated ())
	{
		MeshResource::Destroy ();
	}
}

void MeshResource::SetBuffers (ID3D11DeviceContext & _deviceContext) const
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
#if GAME_MESH_HALF_INDEX_PREC
	GAME_COMC (_deviceContext.IASetIndexBuffer (m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0));
#else
	GAME_COMC (_deviceContext.IASetIndexBuffer (m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0));
#endif
	UINT offsets[] { 0 };
	UINT strides[] { sizeof (Mesh::Vertex) };
	ID3D11Buffer * buffers[] { m_pVertexBuffer };
	GAME_COMC (_deviceContext.IASetVertexBuffers (0, 1, buffers, strides, offsets));
}

void MeshResource::Create (ID3D11Device & _device)
{
	GAME_ASSERT_MSG (pMesh, "Mesh is nullptr");
	GAME_ASSERT_MSG (!IsCreated (), "Already created");
	{
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = static_cast<UINT>(pMesh->GetVerticesSize ());
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof (Mesh::Vertex);
		desc.Usage = D3D11_USAGE_DEFAULT;
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = pMesh->GetVertices ();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;
		GAME_COMC (_device.CreateBuffer (&desc, &data, &m_pVertexBuffer));
	}
	{
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = static_cast<UINT>(pMesh->GetIndicesSize ());
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof (Mesh::ind_t);
		desc.Usage = D3D11_USAGE_DEFAULT;
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = pMesh->GetIndices ();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;
		GAME_COMC (_device.CreateBuffer (&desc, &data, &m_pIndexBuffer));
	}
}

void MeshResource::Destroy ()
{
	GAME_ASSERT_MSG (MeshResource::IsCreated (), "Not created");
	m_pVertexBuffer->Release ();
	m_pIndexBuffer->Release ();
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
}

bool MeshResource::IsCreated () const
{
	return m_pIndexBuffer != nullptr;
}

