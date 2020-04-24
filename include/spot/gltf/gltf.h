#pragma once

#include <spot/math/math.h>
#include <spot/math/shape.h>
#include <nlohmann/json.hpp>

#include "spot/gltf/buffer.h"
#include "spot/gltf/camera.h"
#include "spot/gltf/image.h"
#include "spot/gltf/light.h"
#include "spot/gltf/material.h"
#include "spot/gltf/mesh.h"
#include "spot/gltf/node.h"
#include "spot/gltf/sampler.h"
#include "spot/gltf/script.h"
#include "spot/gltf/texture.h"
#include "spot/gltf/bounds.h"
#include "spot/gltf/animation.h"

namespace spot::gltf
{

class Gltf;

/// Typed view into a bufferView
struct Accessor
{
	/// Datatype of components in the attribute
	enum class ComponentType
	{
		BYTE           = 5120,
		UNSIGNED_BYTE  = 5121,
		SHORT          = 5122,
		UNSIGNED_SHORT = 5123,
		UNSIGNED_INT   = 5125,
		FLOAT          = 5126
	};

	/// Specifies if the attribute is a scalar, vector, or matrix
	enum class Type
	{
		NONE,
		SCALAR,
		VEC2,
		VEC3,
		VEC4,
		MAT2,
		MAT3,
		MAT4
	};

	Accessor( Gltf& m );

	Accessor( Accessor&& a );

	/// @return The size of the data pointed by this accessor
	size_t get_size() const;

	/// @return The address of the data pointed by this accessor
	const uint8_t* get_data() const;

	/// @return The stride of the buffer view pointed by this accessor
	size_t get_stride() const;
	
	/// The model of the accessor
	Gltf* model = nullptr;

	/// Index of the buffer view
	size_t buffer_view_index;

	/// @return The buffer view pointed by this accessor
	BufferView& get_buffer_view() const;

	/// Offset relative to the start of the bufferView in bytes
	size_t byte_offset = 0;

	/// Datatype of components in the attribute
	ComponentType component_type;

	/// Number of attributes referenced by this accessor
	size_t count;

	/// Specifies if the attribute is a scalar, vector, or matrix
	Type type;

	/// Maximum value of each component in this attribute
	std::vector<float> max;

	/// Minimum value of each component in this attribute
	std::vector<float> min;
};

/// GL Transmission Format
class Gltf
{
  public:
	/// Metadata about the glTF asset
	struct Asset
	{
		/// glTF version that this asset targets
		std::string version;
		/// Tool that generated this glTF model. Useful for debugging
		std::string generator;
		/// Copyright message suitable for display to credit the content creator
		std::string copyright;
	};

	friend class Node;
	friend class Scene;

	Gltf() = default;

	/// Move contructs a Gltf object
	/// @param g Gltf object
	Gltf( Gltf&& g );

	/// Move assign a Gltf object
	/// @param g Gltf object
	Gltf& operator=( Gltf&& g );

	/// Constructs a Gltf object
	/// @param j Json object describing the model
	/// @param path Gltf file path
	Gltf( const nlohmann::json& j, const std::string& path = "." );

	/// Delete copy constructor
	Gltf( const Gltf& ) = delete;

	/// Delete copy assignment
	Gltf& operator=( const Gltf& ) = delete;

	/// Loads a GLtf model from path
	/// @param path Gltf file path
	/// @return A Gltf model
	static Gltf load( const std::string& path );

	/// @param i Index of the buffer
	/// @return Buffer number i
	Buffer& get_buffer( const size_t i );

	/// @return A newly created Node
	Handle<Node> create_node();

	/// @param parent The parent of this node
	/// @return A newly created Node
	Handle<Node> create_node( Handle<Node>& parent );

	/// @param name Name of the node
	/// @return A newly created Node
	Handle<Node> create_node( const std::string& name );

	/// @param node Node to add
	Handle<Node> add_node( Node&& node );

	/// @param bounds Index of the bounds
	/// @return The bounds found at that index, nullptr otherwise
	Bounds* get_bounds( int32_t bounds );

	/// @param bounds Index of the bounds
	/// @return The shape of that bounds, nullptr otherwise
	Shape* get_shape( int32_t bounds );

	/// @return The animation at that index, nullptr otherwise
	Accessor* get_accessor( size_t accessor );

	/// Load the nodes pointer using node indices
	void load_nodes();

	/// Load meshes pointers using indices
	void load_meshes();

	/// glTF asset
	Asset asset;

	/// Initializes asset
	/// @param j Json object describing the asset
	void init_asset( const nlohmann::json& j );

	/// Initializes buffers
	/// @param j Json object describing the buffers
	void init_buffers( const nlohmann::json& j );

	/// Initializes bufferViews
	/// @param j Json object describing the bufferViews
	void init_buffer_views( const nlohmann::json& j );

	/// Initializes cameras
	/// @param j Json object describing the cameras
	void init_cameras( const nlohmann::json& j );

	/// Initializes samplers
	/// @param j Json object describing the samplers
	void init_samplers( const nlohmann::json& j );

	/// Initializes images
	/// @param j Json object describing the images
	void init_images( const nlohmann::json& j );

	/// Initializes textures
	/// @param j Json object describing the textures
	void init_textures( const nlohmann::json& j );

	/// Initializes accessors
	/// @param j Json object describing the accessors
	void init_accessors( const nlohmann::json& j );

	/// Initializes materials
	/// @param j Json object describing the materials
	void init_materials( const nlohmann::json& j );

	/// Initializes meshes
	/// @param j Json object describing the meshes
	void init_meshes( const nlohmann::json& j );

	/// Initializes lights
	/// @param j Json object describing the lights
	void init_lights( const nlohmann::json& j );

	/// Initializes nodes
	/// @param j Json object describing the nodes
	void init_nodes( const nlohmann::json& j );

	/// Initializes animations
	/// @param j Json object describing the animations
	void init_animations( const nlohmann::json& j );

	/// Initializes shapes
	/// @param j Json object describing the shapes
	void init_shapes( const nlohmann::json& j );

	/// Initializes scripts
	/// @param j Json object describing scripts
	void init_scripts( const nlohmann::json& j );

	/// Initializes scenes
	/// @param j Json object describing the scenes
	void init_scenes( const nlohmann::json& j );

	/// Loads data into a buffer
	/// @param i Index of the buffer
	Buffer& load_buffer( const size_t i );

	/// Directory path of the gltf file
	std::string path;

	/// List of buffers
	std::vector<Buffer> buffers;

	/// Cache of buffers
	std::map<const size_t, std::vector<char>> buffers_cache;

	/// List of buffer views
	std::vector<BufferView> buffer_views;

	/// List of cameras
	std::vector<Camera> cameras;

	/// List of samplers
	Svec<Sampler> samplers;

	/// List of images
	std::vector<Image> images;

	/// List of textures
	std::vector<Texture> textures;

	/// List of accessors
	std::vector<Accessor> accessors;

	/// List of materials
	std::vector<Material> materials;

	/// List of meshes
	Svec<Mesh> meshes;

	/// List of lights
	std::vector<Light> lights;

	/// List of nodes
	Svec<Node> nodes;

	/// List of animations
	std::vector<Animation> animations;

	/// List of shapes (abstract)
	std::vector<gltf::Rect> rects;
	std::vector<gltf::Box> boxes;
	std::vector<gltf::Sphere> spheres;
	std::vector<gltf::Bounds> bounds;

	/// List of scripts
	std::vector<Script> scripts;

	/// List of scenes
	std::vector<Scene> scenes;

	/// Current scene
	Scene* scene = nullptr;
};


template <typename T>
T from_string( const std::string& s );

template <>
Accessor::Type from_string<Accessor::Type>( const std::string& s );

template <>
Mesh::Primitive::Semantic from_string<Mesh::Primitive::Semantic>( const std::string& s );

template <>
Animation::Sampler::Interpolation from_string<Animation::Sampler::Interpolation>( const std::string& i );

template <>
Animation::Target::Path from_string<Animation::Target::Path>( const std::string& p );

template <>
Bounds::Type from_string<Bounds::Type>( const std::string& b );

template <typename T>
std::string to_string( const T& t );

template <>
std::string to_string<Accessor::Type>( const Accessor::Type& t );

template <>
std::string to_string<Mesh::Primitive::Semantic>( const Mesh::Primitive::Semantic& s );

template <>
std::string to_string<Sampler::Filter>( const Sampler::Filter& f );

template <>
std::string to_string<Sampler::Wrapping>( const Sampler::Wrapping& w );

template <>
std::string to_string<Mesh::Primitive::Mode>( const Mesh::Primitive::Mode& m );

}  // namespace spot::gltf
