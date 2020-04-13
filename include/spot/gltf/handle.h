#pragma once

#include <limits>

using handle_t = std::size_t;

/// @brief Handle macro to declare Handle inner classes
/// Invalid handle is `0`, therefore:
/// - `v + 1` is assigned to value on instantiation
/// - `value - 1` is returned on implicit conversion to `handle_t`
#define HANDLE class Handle { \
  public: \
	Handle() = default; \
	explicit Handle( handle_t v ) : value { v } {} \
	operator handle_t() const { return value; } \
  private: \
	handle_t value = std::numeric_limits<size_t>::max(); \
}
