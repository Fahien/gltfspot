#ifndef GST_IMAGE_H_
#define GST_IMAGE_H_

#include <string>

namespace gltfspot
{
/// Image data used to create a texture
struct Image
{
	/// Uri of the image
	std::string uri = "";

	/// MIME type
	std::string mime_type = "";

	/// Buffer view index
	uint32_t buffer_view = 0;

	/// Name
	std::string name = "Unknown";
};


}  // namespace gltfspot


#endif  // GST_IMAGE_H_