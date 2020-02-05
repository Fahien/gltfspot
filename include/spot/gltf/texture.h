#pragma once

#include <string>

namespace spot::gltf
{

struct Sampler;
struct Image;
class Gltf;


/// Texture and its sampler
struct Texture
{
	Gltf* model = nullptr;

	/// Sampler used by this texture
	Sampler* sampler = nullptr;

	/// Image used by this texture
	int32_t source_index = -1;

	Image* get_source() const;

	/// User-defined name of this object
	std::string name = "Unknown";
};


}  // namespace spot::gltf
