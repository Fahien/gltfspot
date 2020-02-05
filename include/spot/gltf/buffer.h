#pragma once

#include <string>
#include <vector>


namespace spot::gltf
{


class Gltf;


/// Buffer pointing to binary geometry, animation, or skins
struct Buffer
{
	/// Length of the buffer in bytes
	size_t byte_length;

	/// Uri of the buffer
	std::string uri;

	/// Bytes lazily loaded
	std::vector<char> data;
};


/// View into a buffer, generally representing a subset of the buffer
struct BufferView
{
	/// Target that the GPU buffer should be bound to
	enum class Target
	{
		None,
		ArrayBuffer        = 34962,
		ElementArrayBuffer = 34963
	};

	Gltf* model = nullptr;

	/// Index of the buffer
	size_t buffer_index = 0;

	/// Returns the buffer pointed by this buffer view
	Buffer& get_buffer() const;

	/// Offset into the buffer in bytes
	size_t byte_offset = 0;

	/// Length of the bufferView in bytes
	size_t byte_length = 0;

	/// Stride, in bytes
	size_t byte_stride = 0;

	/// Target that the GPU buffer should be bound to
	Target target = Target::None;
};


} // namespace spot::gltf
