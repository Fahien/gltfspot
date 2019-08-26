#ifndef GST_MATERIAL_H_
#define GST_MATERIAL_H_

#define PYSPOT_EXPORT __attribute__( ( annotate( "pyspot" ) ) )

#include <string>
#include <vector>

namespace gltfspot
{
struct Texture;

/// Material appearance of a primitive
struct PYSPOT_EXPORT Material
{
	/// Metallic-Roughness Material
	struct PbrMetallicRoughness
	{
		/// Base color of the material
		std::vector<float> base_color_factor = { 1.0f, 1.0f, 1.0f, 1.0f };

		/// Base color texture
		Texture* base_color_texture = nullptr;

		/// Metalness of the material
		float metallic_factor = 1.0f;

		/// Roughness of the material
		float roughness_factor = 1.0f;
	};

	/// User-defined name of this object
	std::string name = "Unknown";

	/// Metallic-Roughness Material
	PbrMetallicRoughness pbr_metallic_roughness;
};

}  // namespace gltfspot

#endif  // GST_MATERIAL_H_