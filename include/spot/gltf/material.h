#pragma once

#include <string>
#include <vector>

namespace spot::gltf
{
struct Texture;
class Gltf;

/// Material appearance of a primitive
struct Material
{
	/// Metallic-Roughness Material
	struct PbrMetallicRoughness
	{
		/// Base color of the material
		std::vector<float> base_color_factor = { 1.0f, 1.0f, 1.0f, 1.0f };

		/// Base color texture
		int32_t texture_index = -1;

		/// Metalness of the material
		float metallic_factor = 1.0f;

		/// Roughness of the material
		float roughness_factor = 1.0f;
	};

	/// User-defined name of this object
	std::string name = "Unknown";

	/// Metallic-Roughness Material
	PbrMetallicRoughness pbr;

	Gltf* model = nullptr;

	/// @return The texture of the material, otherwise null
	Texture* get_texture() const;

};

}  // namespace spot::gltf
