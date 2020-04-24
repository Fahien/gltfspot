#pragma once

#include <vector>
#include <memory>

namespace spot::gltf
{

template <typename T>
using svec = std::shared_ptr<std::vector<T>>;


/// @brief Handle class using CRTP
template <typename T>
class Handle
{
  public:
	Handle() = default;

	Handle( svec<T>& v, size_t i )
	: vec { v }
	, index { i }
	{
		assert( *this && "Handle is not valid" );
	}

	explicit operator bool() const
	{
		return !vec.expired() && index < vec.lock()->size();
	}

	T* operator->() const { return &**this; }
	T& operator*() const { assert( *this && "Handle is not valid" ); return ( *vec.lock() )[index]; }

	bool operator==( const Handle<T>& other ) const { return !vec.expired() && !other.vec.expired() && vec.lock() == other.vec.lock() && index == other.index; }
	bool operator!=( const Handle<T>& other ) const { return !( *this == other ); }

	size_t get_index() const { return index; }
  private:
	std::weak_ptr<std::vector<T>> vec;
	size_t index = 0;
};


} // namespace spot::gltf

namespace std
{

template <typename T>
struct hash<spot::gltf::Handle<T>>
{
	size_t operator()( const spot::gltf::Handle<T>& handle ) const
	{
		return handle.get_index();
	}
};

}