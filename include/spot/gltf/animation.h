#pragma once

#include <string>
#include <vector>
#include <spot/math/math.h>
#include "spot/gltf/node.h"

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
		Node::Handle node = {};
		/// Property of the node to animate
		Path path;
	};

	/// Input and output accessors with an interpolation
	/// algorithm which define a keyframe graph
	struct Sampler
	{
		HANDLE;

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

	/// Animation sampler at a node property
	struct Channel
	{
		/// Index of the sampler
		Sampler::Handle sampler = {};
		/// Target of the animation
		Target target;
	};

	Animation( Gltf& m ) : model { &m } {}

	/// @return The max keyframe time of the animation
	float find_max_time();

	/// @return The last rotation of the animation
	math::Quat find_last_rotation() const;

	/// @return The list of keyframe time for the sampler
	std::vector<float> get_times( Sampler::Handle sampler ) const;

	/// @return The list of rotations for the sampler
	std::vector<math::Quat> get_rotations( Sampler::Handle sampler ) const;

	/// @brief Adds a new rotation to this animation
	/// @param times Keyframe times to use
	/// @param quats Rotations for each keyframe
	void add_rotation( Node::Handle node, const std::vector<float>& times, const std::vector<math::Quat>& quats );

	/// @brief Adds a new rotation to this animation by using previous values
	/// With no previous values, 0.0f and Quat::Identity will be used
	void add_rotation( Node::Handle node, const float time, const math::Quat& quat );

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
