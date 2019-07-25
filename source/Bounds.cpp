#include "gltfspot/Bounds.h"
#include "gltfspot/Node.h"
#include "gltfspot/Script.h"

#include <cmath>

namespace gltfspot
{
void Shape::set_matrix( const mathspot::Mat4& m )
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


Box::Box( const mathspot::Vec3& aa, const mathspot::Vec3& bb )
    : a{ aa }
    , b{ bb }
{
}

bool Box::intersects( const Shape& s ) const
{
	return s.intersects( *this );
}


bool Box::intersects( const Box& other ) const
{
	auto temp_a = matrix * a;
	auto temp_b = matrix * b;

	auto other_a = other.matrix * other.a;
	auto other_b = other.matrix * other.b;

	auto x      = std::min( temp_a.x, temp_b.x );
	auto y      = std::min( temp_a.y, temp_b.y );
	auto width  = std::fabs( temp_b.x - temp_a.x );
	auto height = std::fabs( temp_b.y - temp_a.y );

	auto other_x      = std::min( other_a.x, other_b.x );
	auto other_y      = std::min( other_a.y, other_b.y );
	auto other_width  = std::fabs( other_b.x - other.a.x );
	auto other_height = std::fabs( other_b.y - other_a.y );

	// TODO add depth
	return ( fabs( x - other_x ) * 2 < ( width + other_width ) ) && ( fabs( y - other_y ) * 2 < ( height + other_height ) );
}


Sphere::Sphere( const mathspot::Vec3& oo, const float rr )
    : o{ oo }
    , r{ rr }
{
}


}  // namespace gltfspot
