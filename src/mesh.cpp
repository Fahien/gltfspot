#include "spot/gltf/mesh.h"

#include "spot/gltf/gltf.h"

namespace spot::gltf
{


Mesh::Primitive::Primitive( Mesh& m )
: mesh { &m }
{}


Mesh::Primitive::Primitive( Mesh::Primitive&& other )
: mesh { other.mesh }
, attributes { std::move( other.attributes ) }
, indices_index { other.indices_index }
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
	indices_index = other.indices_index;
	material_index = other.material_index;
	material = other.material;
	mode = other.mode;
	extras = other.extras;

	other.mesh     = nullptr;
	other.material = nullptr;
	other.extras   = nullptr;

	return *this;
}


std::unordered_map<Mesh::Primitive::Semantic, Accessor*> Mesh::Primitive::get_attributes()
{
	std::unordered_map<Mesh::Primitive::Semantic, Accessor*> ret;

	for ( auto [primitive, accessor_index] : attributes )
	{
		ret.emplace( primitive, &mesh->model->accessors[accessor_index]);
	}

	return ret;
}


Accessor* Mesh::Primitive::get_indices() const
{
	if ( indices_index >= 0 )
	{
		return& mesh->model->accessors[indices_index];
	}
	return nullptr;
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


} // namespace spot::gltf
