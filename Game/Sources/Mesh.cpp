#include <Game/Rendering/Mesh.hpp>

Mesh::Mesh (const std::vector<Vertex>& _vertices, const std::vector<ind_t>& _indices)
	: vertices { _vertices }, indices { _indices }
{}

unsigned int Mesh::GetIndicesCount () const
{
	return static_cast<unsigned int>(indices.size ());
}

unsigned int Mesh::GetVerticesCount () const
{
	return static_cast<unsigned int>(vertices.size ());
}

const Mesh::Vertex * Mesh::GetVertices () const
{
	return vertices.data ();
}

const Mesh::ind_t * Mesh::GetIndices () const
{
	return indices.data ();
}
