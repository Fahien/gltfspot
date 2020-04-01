#include "spot/gltf/bounds.h"

namespace spot::gltf
{


void Shape::set_matrix( const math::Mat4& m )
{
	matrix = m;
}


bool Shape::is_colliding_with( const Shape& s ) const
{
	auto it = std::find( std::begin( collisions ), std::end( collisions ), &s );
	return it != std::end( collisions );
}


void Shape::add_collision( const Shape& s )
{
	collisions.push_back( &s );
}


void Shape::remove_collision( const Shape& s )
{
	auto it = std::find( std::begin( collisions ), std::end( collisions ), &s );
	if ( it != std::end( collisions ) )
	{
		collisions.erase( it );
	}
}


} // namespace spot::gltf
