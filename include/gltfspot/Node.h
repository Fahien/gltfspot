#ifndef GST_NODE_H_
#define GST_NODE_H_

#include <mathspot/Math.h>
#include <vector>

namespace gltfspot
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
	/// Gltf owning the node
	Gltf* gltf = nullptr;

	/// Index of this node within the nodes vector
	size_t index = 0;

	/// Parent of this node
	Node* parent = nullptr;

	/// Camera referenced by this node
	Camera* camera = nullptr;

	/// This node's children
	std::vector<Node*> children;

	// This node's children indices
	std::vector<size_t> children_indices;

	/// Floating-point 4x4 transformation matrix stored in column-major order
	mathspot::Mat4 matrix = mathspot::Mat4::identity;

	/// Mesh in this node
	Mesh* mesh = nullptr;

	/// Unit quaternion rotation in the order (x, y, z, w), where w is the scalar
	mathspot::Quat rotation = mathspot::Quat::identity;

	/// Non-uniform scale
	mathspot::Vec3 scale = mathspot::Vec3{ 1.0f, 1.0f, 1.0f };

	/// Translation
	mathspot::Vec3 translation = mathspot::Vec3{ 0.0f, 0.0f, 0.0f };

	/// User-defined name of this object
	std::string name = "Unknown";

	/// If not negative, index of light contained by this node
	int32_t light_index = -1;

	/// Light referenced by this node
	Light* light = nullptr;

	/// This node's bounds indices
	int32_t bounds_index = -1;

	/// Bounds referenced by this node
	Shape* bounds = nullptr;

	/// This node's scripts indices
	std::vector<size_t> scripts_indices;

	/// This node's scripts
	std::vector<Script*> scripts;

	/// @return A newly created Node as a child of this
	/// @param[in] name Name of the node
	Node& create_child( const std::string& name );

	void remove_from_parent();
};


}  // namespace gltfspot

#endif  // GST_NODE_H_
