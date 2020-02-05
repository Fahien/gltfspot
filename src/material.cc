#include "spot/gltf/material.h"

#include "spot/gltf/gltf.h"


namespace spot::gltf
{


Texture* Material::get_texture() const
{
	if ( pbr.texture_index >= 0 )
	{
		return &model->textures[pbr.texture_index];
	}
	return nullptr;
}


} // namespace spot::gltf
