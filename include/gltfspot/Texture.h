#ifndef GST_TEXTURE_H_
#define GST_TEXTURE_H_

#include <string>

namespace gltfspot
{
struct Sampler;
struct Image;

/// Texture and its sampler
struct Texture
{
	/// Sampler used by this texture
	Sampler* sampler = nullptr;

	/// Image used by this texture
	Image* source = nullptr;

	/// User-defined name of this object
	std::string name = "Unknown";
};

}  // namespace gltfspot


#endif  // GST_TEXTURE_H_