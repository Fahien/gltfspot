#include "spot/gltf/texture.h"

#include "spot/gltf/gltf.h"

namespace spot::gltf
{

Image* Texture::get_source() const
{
	if ( source_index >= 0 )
	{
		return &model->images[source_index];
	}
	return nullptr;
}

}