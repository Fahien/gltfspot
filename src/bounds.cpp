#include "spot/gltf/bounds.h"
#include "spot/gltf/node.h"
#include "spot/gltf/script.h"

#include <cmath>
#include <algorithm>

namespace spot::gltf
{
void Shape::set_matrix( const spot::math::Mat4& m )
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


Box::Box( const spot::math::Vec3& aa, const spot::math::Vec3& bb )
    : a{ aa }
    , b{ bb }
{
}

bool Box::intersects( const Shape& s ) const
{
	return s.intersects( *this );
}


spot::math::Vec3 Box::get_abs_a() const
{
	return matrix * a;
}


spot::math::Vec3 Box::get_abs_b() const
{
	return matrix * b;
}


bool Box::intersects( const Box& other ) const
{
	auto temp_a = get_abs_a();
	auto temp_b = get_abs_b();

	auto other_a = other.get_abs_a();
	auto other_b = other.get_abs_b();

	// TODO add depth
	return temp_a.x < other_b.x && temp_b.x > other_a.x && temp_b.y > other_a.y && temp_a.y < other_b.y;
}


Sphere::Sphere( const spot::math::Vec3& oo, const float rr )
    : o{ oo }
    , r{ rr }
{
}


}  // namespace spot::gltf
