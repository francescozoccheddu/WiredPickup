#pragma once

#define GAME_MESH_HALF_INDEX_PREC 1

#include <Game/DirectXMath.hpp>
#include <cstdint>
#include <vector>

class MeshProvider
{

public:

	struct Vertex
	{
		DirectX::XMVECTOR position;
	};

#if GAME_MESH_HALF_INDEX_PREC 
	using ind_t = uint16_t;
#else 
	using ind_t = uint32_t;
#endif

	virtual ~MeshProvider () = default;

	virtual unsigned int GetIndicesCount () const = 0;

	virtual unsigned int GetVerticesCount () const = 0;

	virtual const Vertex * GetVertices () const = 0;

	virtual const ind_t * GetIndices () const = 0;

	inline size_t GetIndicesSize () const
	{
		return sizeof (ind_t) * GetIndicesCount ();
	}

	inline size_t GetVerticesSize () const
	{
		return sizeof (Vertex) * GetVerticesCount ();
	}

};

class Mesh final : public MeshProvider
{

public:

	Mesh () = default;

	Mesh (const std::vector<Vertex>& vertices, const std::vector<ind_t>& indices);

	std::vector<Vertex> vertices;
	std::vector<ind_t> indices;

	virtual unsigned int GetIndicesCount () const override;

	virtual unsigned int GetVerticesCount () const override;

	virtual const Vertex * GetVertices () const override;

	virtual const ind_t * GetIndices () const override;

};