#pragma once

#include <vector>
#include <memory>

namespace spot::gltf
{


template <typename T>
class Uvec : public std::unique_ptr<std::vector<T>>
{
  public:
	Uvec() : std::unique_ptr<std::vector<T>>( std::make_unique<std::vector<T>>() ) {}
};


template <typename T>
class Handle
{
  public:
	Handle() = default;

	Handle( Uvec<T>& v, size_t i )
	: vec { v.get() }
	, index { i }
	{
		assert( *this && "Handle is not valid" );
	}

	explicit operator bool() const
	{
		return vec && index < vec->size();
	}

	T* operator->() const { return &**this; }
	T& operator*() const { assert( *this && "Handle is not valid" ); return ( *vec )[index]; }

	bool operator==( const Handle<T>& other ) const { return vec == other.vec && index == other.index; }
	bool operator!=( const Handle<T>& other ) const { return !( *this == other ); }

	size_t get_index() const { return index; }

  private:
	std::vector<T>* vec = nullptr;
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