#pragma once

#include <spot/math/math.h>
#include <vector>

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
	int32_t index = -1;

	/// Parent of this node
	int32_t parent = -1;

	/// @return The parent of this node, otherwise nullptr
	Node* get_parent() const;

	/// Camera referenced by this node
	Camera* camera = nullptr;

	/// This node's children indices
	std::vector<int32_t> children;

	/// @return A list of children of this node
	std::vector<Node*> get_children() const;

	/// Floating-point 4x4 transformation matrix stored in column-major order
	spot::math::Mat4 matrix = spot::math::Mat4::identity;

	/// If not negative, index of mesh of the node
	int32_t mesh = -1;

	/// @return The mesh associated to this node, null otherwise
	Mesh* get_mesh() const;

	/// Unit quaternion
	spot::math::Quat rotation = spot::math::Quat::identity;

	/// Non-uniform scale
	spot::math::Vec3 scale = spot::math::Vec3{ 1.0f, 1.0f, 1.0f };

	/// Translation
	spot::math::Vec3 translation = spot::math::Vec3{ 0.0f, 0.0f, 0.0f };

	/// User-defined name of this object
	std::string name = "Unknown";

	/// If not negative, index of light contained by this node
	int32_t light_index = -1;

	/// Light referenced by this node
	Light* light = nullptr;

	/// This node's bounds indices
	int32_t bounds = -1;

	/// This node's scripts indices
	std::vector<size_t> scripts_indices;

	/// This node's scripts
	std::vector<Script*> scripts;

	/// @return A newly created Node as a child of this
	/// @param[in] name Name of the node
	Node& create_child( const std::string& name );

	void remove_from_parent();
};


/// Root nodes of a scene
struct Scene
{
	/// Gltf owning the scene
	Gltf* model = nullptr;
	
	/// Indices of each root node
	std::vector<int32_t> nodes;
	
	/// User-defined name of this object
	std::string name = "default";

	/// @return A newly created Node as root of a scene
	/// @param[in] name Name of the node
	Node& create_node( const std::string& name = {});
};


}  // namespace spot::gltf
