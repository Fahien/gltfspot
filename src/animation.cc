#include "spot/gltf/animation.h"

#include <cassert>

#include "spot/gltf/gltf.h"

namespace spot::gltf
{


void Animation::add_rotation(
	const int32_t node,
	const std::vector<float>& times,
	const std::vector<math::Quat>& quats )
{
	assert( node >= 0 && "Animation should work on a valid node" );
	assert( times.size() == quats.size() && "Times and quats count does not match" );

	// Create a channel to target the node
	auto& channel = channels.emplace_back();
	channel.target.node = node;
	channel.target.path = gltf::Animation::Target::Path::Rotation;
	channel.sampler = samplers.size();

	// Create the sampler
	auto& sampler = samplers.emplace_back();
	sampler.interpolation = gltf::Animation::Sampler::Interpolation::Linear;

	// Timepoints
	{
		sampler.input = model->accessors.size();

		auto& accessor = model->accessors.emplace_back( *model );
		accessor.count = times.size();
		accessor.buffer_view_index = model->buffer_views.size();

		auto& buffer_view = model->buffer_views.emplace_back();
		buffer_view.buffer_index = model->buffers.size();

		auto& buffer = model->buffers.emplace_back();
		buffer.byte_length = times.size() * sizeof( float );
		buffer.data.resize( buffer.byte_length );
		std::memcpy( buffer.data.data(), times.data(), buffer.byte_length );
	}

	// Rotation values
	{
		sampler.output = model->accessors.size();

		auto& accessor = model->accessors.emplace_back( *model );
		accessor.count = quats.size();
		accessor.buffer_view_index = model->buffer_views.size();

		auto& buffer_view = model->buffer_views.emplace_back();
		buffer_view.buffer_index = model->buffers.size();

		auto& buffer = model->buffers.emplace_back();
		buffer.byte_length = quats.size() * sizeof( math::Quat );
		buffer.data.resize( buffer.byte_length );
		std::memcpy( buffer.data.data(), quats.data(), buffer.byte_length );
	}
}


} // namespace spot::gltf
