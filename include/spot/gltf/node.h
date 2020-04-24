#pragma once

#include <spot/math/math.h>
#include <vector>

#include "spot/gltf/handle.h"


namespace spot::gltf
{
class Gltf;
class Camera;
class Mesh;
class Light;
class Script;
class Shape;


/// Node in the node hierarchy
class Node
{
  public:
	bool contains( const math::Vec2& point ) const;

	/// Gltf owning the node
	Gltf* model = nullptr;

	/// Index of this node within the nodes vector
	Handle<Node> handle = {};

	/// Parent of this node
	Handle<Node> parent = {};

	/// @return The parent of this node, otherwise nullptr
	Handle<Node> get_parent() const;

	/// Camera referenced by this node
	Camera* camera = nullptr;

	/// @return A list of children of this node
	std::vector<Handle<Node>>& get_children() { return children; }

	/// @return The current transform
	math::Mat4 get_matrix() const;

	/// @return A matrix representing the absolute transform of this node
	math::Mat4 get_absolute_matrix() const;

	/// If not negative, index of mesh of the node
	int32_t mesh = -1;

	/// @return The mesh associated to this node, null otherwise
	Handle<Mesh> get_mesh() const;

	/// Unit quaternion
	math::Quat rotation = math::Quat::identity;

	/// Non-uniform scale
	math::Vec3 scale = math::Vec3{ 1.0f, 1.0f, 1.0f };

	/// Translation
	math::Vec3 translation = math::Vec3{ 0.0f, 0.0f, 0.0f };

	/// User-defined name of this object
	std::string name = "Unknown";

	/// If not negative, index of light contained by this node
	int32_t light_index = -1;

	/// This node's bounds indices
	int32_t bounds = -1;

	/// This node's scripts indices
	std::vector<size_t> scripts_indices;

	/// This node's scripts
	std::vector<Script*> scripts;

	/// @param name Name of the node
	/// @return A newly created Node as a child of this
	Handle<Node> create_child( const std::string& name );

	/// @brief Add a new child to this node
	void add_child( Handle<Node>& child );

	void remove_from_parent();

	std::vector<Handle<Node>> children;

  private:
	/// Floating-point 4x4 transformation matrix stored in column-major order
	math::Mat4 matrix = math::Mat4::identity;

	/// This node's children indices

	friend class Gltf;
};


/// Root nodes of a scene
struct Scene
{
	/// Gltf owning the scene
	Gltf* model = nullptr;
	
	/// Indices of each root node
	std::vector<Handle<Node>> nodes;
	
	/// User-defined name of this object
	std::string name = "default";

	/// @param name Name of the node
	/// @return A newly created Node as root of a scene
	Handle<Node> create_node( const std::string& name = {} );
};


}  // namespace spot::gltf
