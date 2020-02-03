#pragma once

#include <vector>
#include <functional>

#include <spot/math/math.h>

namespace gltfspot
{
class Node;
class Box;
class Sphere;

struct Shape
{
	Shape()          = default;
	virtual ~Shape() = default;

	void set_matrix( const spot::math::Mat4& m );

	bool is_colliding_with( const Shape& s ) const;
	void add_collision( const Shape& s );
	void remove_collision( const Shape& s );

	virtual bool intersects( const Shape& s ) const { return false; }
	virtual bool intersects( const Box& b ) const { return false; }
	virtual bool intersects( const Sphere& b ) const { return false; }

	Node* node = nullptr;

	spot::math::Mat4 matrix = {};

	std::vector<const Shape*> collisions = {};

	std::function<void(const Shape& s)> start_colliding_with = {};
	std::function<void(const Shape& s)> colliding_with = {};
	std::function<void(const Shape& s)> end_colliding_with = {};
};


struct Box : public Shape
{
	Box( const spot::math::Vec3& a = {}, const spot::math::Vec3& b = {} );

	spot::math::Vec3 get_abs_a() const;
	spot::math::Vec3 get_abs_b() const;

	bool intersects( const Shape& s ) const override;
	bool intersects( const Box& b ) const override;

	spot::math::Vec3 a = {};
	spot::math::Vec3 b = {};
};


struct Sphere : public Shape
{
	Sphere( const spot::math::Vec3& o = {}, float r = 0.0f );

	spot::math::Vec3 o = {};
	float          r = 0.0f;
};

struct Bounds
{
	enum class Type
	{
		Box,
		Sphere
	};

	Shape* shape = nullptr;
};

}  // namespace gltfspot
