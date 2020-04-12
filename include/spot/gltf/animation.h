#pragma once

#include <string>
#include <vector>
#include <spot/math/math.h>

namespace spot::gltf
{


class Gltf;


/// Keyframe animation
struct Animation
{
	/// Identifies which node to animate
	struct Target
	{
		enum class Path
		{
			None,
			Translation,
			Rotation,
			Scale,
			Weights
		};

		/// Index of the target node
		int32_t node = -1;
		/// Property of the node to animate
		Path path;
	};

	/// Animation sampler at a node property
	struct Channel
	{
		/// Index of the sampler
		size_t sampler = 0;
		/// Target of the animation
		Target target;
	};

	/// Input and output accessors with an interpolation
	/// algorithm which define a keyframe graph
	struct Sampler
	{
		/// Interpolation algorithm
		enum class Interpolation
		{
			Linear,
			Step,
			Cubicspline
		};

		/// Index of accessor with keyframe input
		size_t input;
		/// Index of accessor with keyframe output
		size_t output;
		/// Interpolation method used between keyframes
		Interpolation interpolation = Interpolation::Linear;
	};

	Animation( Gltf& m ) : model { &m } {}

	/// @brief Adds a new rotation to this animation
	void add_rotation( const int32_t node, const std::vector<float>& times, const std::vector<math::Quat>& quats );

	Gltf* model = nullptr;

	/// Name of the animation
	std::string name = "Unknown";

	/// Keeps track of time internally
	struct Time
	{
		float current = 0.0f;
		float max     = 0.0f;
	} time;

	/// Channels
	std::vector<Channel> channels;

	/// Samplers
	std::vector<Sampler> samplers;
};


} // namespace spot::gltf
