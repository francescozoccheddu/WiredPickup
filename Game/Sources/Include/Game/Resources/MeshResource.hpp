#pragma once

#include <Game/Resources/Resource.hpp>
#include <Game/Direct3D.hpp>
#include <Game/Rendering/Mesh.hpp>

class MeshResource final : public Resource
{

public:

	~MeshResource () override;

	inline constexpr static const D3D11_INPUT_ELEMENT_DESC s_aInputElementDesc[] { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof (Mesh::Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	void SetBuffers (ID3D11DeviceContext & deviceContext) const;

	void Create (ID3D11Device & device) override final;

	void Destroy () override final;

	bool IsCreated () const override final;

	const MeshProvider * pMesh { nullptr };

private:

	friend class VertexShaderResource;

	ID3D11Buffer * m_pVertexBuffer { nullptr };
	ID3D11Buffer * m_pIndexBuffer { nullptr };

};