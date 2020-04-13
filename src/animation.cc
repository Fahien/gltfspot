#include "spot/gltf/animation.h"

#include <cassert>

#include "spot/gltf/gltf.h"

namespace spot::gltf
{


std::vector<math::Quat> Animation::get_rotations( const size_t sampler_index ) const
{
	std::vector<math::Quat> quats;

	if ( sampler_index < samplers.size() )
	{
		auto& sampler = samplers[sampler_index];
		auto& accessor = model->accessors.at( sampler.output );

		auto len = accessor.count * sizeof( math::Quat );
		assert( accessor.get_size() == len && "Size mismatch" );

		quats.resize( accessor.count );
		std::memcpy( quats.data(), accessor.get_data(), len );
	}

	return quats;
}


math::Quat Animation::find_last_rotation() const
{
	for ( int32_t i = channels.size() - 1; i>= 0; --i )
	{
		auto& channel = channels[i];
		if ( channel.target.path == Target::Path::Rotation )
		{
			auto quats = get_rotations( channel.sampler );
			if ( quats.size() )
			{
				return quats.back();
			}
		}
	}

	return math::Quat::identity;
}


std::vector<float> Animation::get_times( const size_t sampler_index ) const
{
	std::vector<float> times;

	if ( sampler_index < samplers.size() )
	{
		auto& sampler = samplers[sampler_index];
		if ( auto accessor = model->get_accessor( sampler.input ) )
		{
			times.resize( accessor->count );
			std::memcpy( times.data(), accessor->get_data(), accessor->count * sizeof( float ) );
		}
	}

	return times;
}


float Animation::find_max_time()
{
	for ( auto& channel : channels )
	{
		std::vector<float> times = get_times( channel.sampler );

		auto it = std::max_element( std::begin( times ), std::end( times ) );
		if ( it != std::end( times ) )
		{
			time.max = std::max<float>( time.max, *it );
		}
	}

	return time.max;
}


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
		accessor.type = Accessor::Type::SCALAR;
		accessor.component_type = Accessor::ComponentType::FLOAT;
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
		accessor.type = Accessor::Type::VEC4;
		accessor.component_type = Accessor::ComponentType::FLOAT;
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


void Animation::add_rotation( const int32_t node, const float time, const math::Quat& quat )
{
	assert( node >= 0 && "Animation should work on a valid node" );

	// Timepoints
	auto max_time = find_max_time();
	assert( time > max_time && "Target time should be greater than current max time" );
	std::vector<float> times = { max_time, time };

	// Rotation values
	auto last_rotation = find_last_rotation();
	std::vector<math::Quat> quats = { last_rotation, quat };

	// Create a channel to target the node
	auto& channel = channels.emplace_back();
	channel.target.node = node;
	channel.target.path = Target::Path::Rotation;
	channel.sampler = samplers.size();

	// Create the sampler
	auto& sampler = samplers.emplace_back();
	sampler.interpolation = gltf::Animation::Sampler::Interpolation::Linear;

	{
		sampler.input = model->accessors.size();

		auto& accessor = model->accessors.emplace_back( *model );
		accessor.type = Accessor::Type::SCALAR;
		accessor.component_type = Accessor::ComponentType::FLOAT;
		accessor.count = times.size();
		accessor.buffer_view_index = model->buffer_views.size();

		auto& buffer_view = model->buffer_views.emplace_back();
		buffer_view.buffer_index = model->buffers.size();

		auto& buffer = model->buffers.emplace_back();
		buffer.byte_length = times.size() * sizeof( float );
		buffer.data.resize( buffer.byte_length );
		std::memcpy( buffer.data.data(), times.data(), buffer.byte_length );
	}

	{
		sampler.output = model->accessors.size();

		auto& accessor = model->accessors.emplace_back( *model );
		accessor.type = Accessor::Type::VEC4;
		accessor.component_type = Accessor::ComponentType::FLOAT;
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
