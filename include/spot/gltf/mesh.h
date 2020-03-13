#ifndef GST_MESH_H_
#define GST_MESH_H_

#include <unordered_map>
#include <string>
#include <vector>


namespace spot::gltf
{
class Gltf;
struct Accessor;
struct Material;

/// Set of primitives to be rendered
struct Mesh
{
	/// Geometry to be rendered with the given material
	struct Primitive
	{
		/// Enumerated value identifying the vertex attribute
		enum class Semantic
		{
			NONE,
			POSITION,
			NORMAL,
			TANGENT,
			TEXCOORD_0,
			TEXCOORD_1,
			COLOR_0,
			JOINTS_0,
			WEIGHTS_0
		};

		/// Type of primitives to render
		enum class Mode
		{
			POINTS,
			LINES,
			LINE_LOOP,
			LINE_STRIP,
			TRIANGLES,
			TRIANGLE_STRIP,
			TRIANGLE_FAN
		};

		Primitive( Mesh& m );

		Primitive( Primitive&& p );

		Primitive& operator=( Primitive&& p );

		/// Mesh owning the primitive
		Mesh* mesh = nullptr;

		/// Dictionary object, where each key corresponds to mesh attribute semantic and
		/// each value is the index of the accessor containing attribute's data (required)
		std::unordered_map<Semantic, uint32_t> attributes;

		std::unordered_map<Semantic, Accessor*> get_attributes();

		/// If >= 0, it is the index of the accessor that contains the indices
		int32_t indices_index = -1;

		/// @return A pointer to the accessor if it has indices, otherwise null
		Accessor* get_indices() const;

		/// Index of the material to apply to this primitive when rendering
		int32_t material = -1;

		/// @return A pointer to the material of this primitive, otherwise null
		Material* get_material() const;

		/// Type of primitives to render
		Mode mode = Mode::TRIANGLES;

		/// targets TODO An array of Morph Targets, each Morph Target is a dictionary mapping attributes (only POSITION,
		/// NORMAL, and TANGENT supported) to their deviations in the Morph Target extensions TODO Dictionary object with
		/// extension-specific objects Application-specific data
		void* extras;
	};

	Mesh( Gltf& g );

	Mesh( Mesh&& m );

	/// Gltf model owning the mesh
	Gltf* model;

	/// Array of primitives, each defining geometry to be rendered with a material (required)
	std::vector<Primitive> primitives;

	/// Array of weights to be applied to the Morph Targets
	std::vector<float> weights;

	/// User-defined name of this object
	std::string name = "Unknown";

	/// extensions TODO Dictionary object with extension-specific objects
	/// extras TODO Application-specific data
	void* extras;
};


}  // namespace spot::gltf


#endif  // GST_MESH_H_