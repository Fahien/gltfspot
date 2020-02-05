#include "spot/gltf/buffer.h"

#include "spot/gltf/gltf.h"

namespace spot::gltf
{


Buffer& BufferView::get_buffer() const
{
	return model->buffers[buffer_index];
}


} // namespace spot::gltf
