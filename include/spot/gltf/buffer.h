#pragma once

#include <string>
#include <vector>

#include "spot/gltf/handle.h"


namespace spot::gfx
{


/// Buffer pointing to binary geometry, animation, or skins
struct ByteBuffer
{
	Handle<ByteBuffer> handle = {};

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

	Handle<BufferView> handle = {};

	/// Index of the buffer
	Handle<ByteBuffer> buffer = {};

	/// Offset into the buffer in bytes
	size_t byte_offset = 0;

	/// Length of the bufferView in bytes
	size_t byte_length = 0;

	/// Stride, in bytes
	size_t byte_stride = 0;

	/// Target that the GPU buffer should be bound to
	Target target = Target::None;
};


} // namespace spot::gfx
