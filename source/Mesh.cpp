#include "gltfspot/Mesh.h"

namespace gltfspot
{


Mesh::Primitive::Primitive( Mesh& m )
: mesh { &m }
{}


Mesh::Primitive::Primitive( Mesh::Primitive&& other )
: mesh { other.mesh }
, attributes { std::move( other.attributes ) }
, indices { other.indices }
, material_index { other.material_index }
, material { other.material }
, mode { other.mode }
, extras { other.extras }
{
	other.mesh     = nullptr;
	other.material = nullptr;
	other.extras   = nullptr;
}


Mesh::Primitive& Mesh::Primitive::operator=( Mesh::Primitive&& other )
{
	mesh = other.mesh;
	attributes = std::move( other.attributes );
	indices = other.indices;
	material_index = other.material_index;
	material = other.material;
	mode = other.mode;
	extras = other.extras;

	other.mesh     = nullptr;
	other.material = nullptr;
	other.extras   = nullptr;

	return *this;
}

Mesh::Mesh( Gltf& g )
: model { &g }
{}


Mesh::Mesh( Mesh&& other )
: model { other.model }
, primitives { std::move( other.primitives ) }
, weights { std::move( other.weights ) }
, name { std::move( other.name ) }
, extras { other.extras }
{
	other.model  = nullptr;
	other.extras = nullptr;

	for ( auto& prim : primitives )
	{
		prim.mesh = this;
	}
}


} // namespace gltfspot
