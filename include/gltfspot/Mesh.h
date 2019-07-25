#ifndef GST_MESH_H_
#define GST_MESH_H_

#include <map>
#include <string>
#include <vector>

#define PYSPOT_EXPORT __attribute__( ( annotate( "pyspot" ) ) )

namespace gltfspot
{
/// Set of primitives to be rendered
struct PYSPOT_EXPORT Mesh
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

		/// Dictionary object, where each key corresponds to mesh attribute semantic and
		/// each value is the index of the accessor containing attribute's data (required)
		std::map<Semantic, unsigned> attributes;

		/// If >= 0, it is the index of the accessor that contains the indices
		int32_t indices = -1;

		/// Index of the material to apply to this primitive when rendering
		unsigned material;

		/// Type of primitives to render
		Mode mode = Mode::TRIANGLES;

		/// targets TODO An array of Morph Targets, each Morph Target is a dictionary mapping attributes (only POSITION,
		/// NORMAL, and TANGENT supported) to their deviations in the Morph Target extensions TODO Dictionary object with
		/// extension-specific objects Application-specific data
		void* extras;
	};

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


}  // namespace gltfspot


#endif  // GST_MESH_H_