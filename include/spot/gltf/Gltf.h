#pragma once

#include <spot/math/math.h>
#include <nlohmann/json.hpp>

#include "spot/gltf/Bounds.h"
#include "spot/gltf/Camera.h"
#include "spot/gltf/Image.h"
#include "spot/gltf/Light.h"
#include "spot/gltf/Material.h"
#include "spot/gltf/Mesh.h"
#include "spot/gltf/Node.h"
#include "spot/gltf/Sampler.h"
#include "spot/gltf/Script.h"
#include "spot/gltf/Texture.h"

namespace spot::gltf
{
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

	/// Buffer pointing to binary geometry, animation, or skins
	struct Buffer
	{
		/// Length of the buffer in bytes
		size_t byteLength;
		/// Uri of the buffer
		std::string uri;
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

		/// Index of the buffer
		size_t buffer = 0;
		/// Offset into the buffer in bytes
		size_t byteOffset = 0;
		/// Length of the bufferView in bytes
		size_t byteLength = 0;
		/// Stride, in bytes
		size_t byteStride = 0;
		/// Target that the GPU buffer should be bound to
		Target target = Target::None;
	};

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

		/// @return The size of the buffer view pointed by this accessor
		size_t get_size() const;

		/// @return The address of the data pointed by this accessor
		const void* get_data() const;

		/// @return The stride of the buffer view pointed by this accessor
		size_t get_stride() const;
		
		/// The model of the accessor
		Gltf* model = nullptr;

		/// Index of the buffer view
		size_t bufferView;

		/// Offset relative to the start of the bufferView in bytes
		size_t byteOffset{ 0 };

		/// Datatype of components in the attribute
		ComponentType componentType;

		/// Number of attributes referenced by this accessor
		size_t count;

		/// Specifies if the attribute is a scalar, vector, or matrix
		Type type;

		/// Maximum value of each component in this attribute
		std::vector<float> max;

		/// Minimum value of each component in this attribute
		std::vector<float> min;
	};

	friend class Node;

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
			int32_t node_index = -1;
			/// Target node
			Node* node = nullptr;
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


		/// Name of the animation
		std::string name = "Unknown";
		/// Keep track of time
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

	/// Root nodes of a scene
	struct Scene
	{
		/// Gltf owning the scene
		Gltf* gltf = nullptr;
		/// Pointers to root nodes to be generated
		std::vector<Node*> nodes;
		/// Indices of each root node
		std::vector<size_t> nodes_indices;
		/// User-defined name of this object
		std::string name = "default";

		/// @return A newly created Node as root of a scene
		/// @param[in] name Name of the node
		Node& create_node( const std::string& name );
	};

	friend class Scene;

	/// Move contructs a Gltf object
	/// @param[in] g Gltf object
	Gltf( Gltf&& g );

	/// Move assign a Gltf object
	/// @param[in] g Gltf object
	Gltf& operator=( Gltf&& g );

	/// Constructs a Gltf object
	/// @param[in] j Json object describing the model
	/// @param[in] path Gltf file path
	Gltf( const nlohmann::json& j, const std::string& path = "." );

	/// Delete copy constructor
	Gltf( const Gltf& ) = delete;

	/// Delete copy assignment
	Gltf& operator=( const Gltf& ) = delete;

	/// Loads a GLtf model from path
	/// @param[in] path Gltf file path
	/// @return A Gltf model
	static Gltf Load( const std::string& path );

	/// @return The Path
	const std::string& GetPath();

	/// @param[in] i Index of the buffer
	/// @return Buffer number i
	std::vector<char>& GetBuffer( const size_t i );

	/// @return Buffer views
	std::vector<BufferView>& GetBufferViews();

	/// @return Cameras
	std::vector<Camera>& get_cameras() { return cameras; }

	/// @return Samplers
	std::vector<Sampler>& get_samplers() { return samplers; }

	/// @return Images
	std::vector<Image>& get_images() { return images; }

	/// @return Textures
	std::vector<Texture>& get_textures() { return textures; }

	/// @return Accessors
	std::vector<Accessor>& GetAccessors();

	/// @return Materials
	std::vector<Material>& GetMaterials();

	/// @return Meshes
	std::vector<Mesh>& get_meshes() { return meshes; }

	/// @return Lights
	std::vector<Light>& get_lights() { return lights; }

	/// @return Animations
	std::vector<Gltf::Animation>& get_animations() { return animations; }

	/// @return Nodes
	std::vector<Node>& get_nodes() { return nodes; }

	/// @return Shapes
	std::vector<std::unique_ptr<Shape>>& get_shapes() { return shapes; }

	/// @return Extras
	std::vector<Script>& get_scripts() { return scripts; }

	/// @return Scenes
	std::vector<Scene>& GetScenes();

	/// @return Current scene
	Scene* GetScene();

	/// @return A newly created Node
	/// @param[in] name Name of the node
	Node create_node( const std::string& name );

	/// @param[in] node Node to add
	Node& add_node( Node&& node );

	/// Load the nodes pointer using node indices
	void load_nodes();

	/// Load meshes pointers using indices
	void load_meshes();

	/// glTF asset
	Asset asset;

  private:
	/// Initializes asset
	/// @param[in] j Json object describing the asset
	void initAsset( const nlohmann::json& j );

	/// Initializes buffers
	/// @param[in] j Json object describing the buffers
	void initBuffers( const nlohmann::json& j );

	/// Initializes bufferViews
	/// @param[in] j Json object describing the bufferViews
	void initBufferViews( const nlohmann::json& j );

	/// Initializes cameras
	/// @param[in] j Json object describing the cameras
	void initCameras( const nlohmann::json& j );

	/// Initializes samplers
	/// @param[in] j Json object describing the samplers
	void initSamplers( const nlohmann::json& j );

	/// Initializes images
	/// @param[in] j Json object describing the images
	void initImages( const nlohmann::json& j );

	/// Initializes textures
	/// @param[in] j Json object describing the textures
	void initTextures( const nlohmann::json& j );

	/// Initializes accessors
	/// @param[in] j Json object describing the accessors
	void initAccessors( const nlohmann::json& j );

	/// Initializes materials
	/// @param[in] j Json object describing the materials
	void initMaterials( const nlohmann::json& j );

	/// Initializes meshes
	/// @param[in] j Json object describing the meshes
	void init_meshes( const nlohmann::json& j );

	/// Initializes lights
	/// @param[in] j Json object describing the lights
	void init_lights( const nlohmann::json& j );

	/// Initializes nodes
	/// @param[in] j Json object describing the nodes
	void init_nodes( const nlohmann::json& j );

	/// Initializes animations
	/// @param[in] j Json object describing the animations
	void init_animations( const nlohmann::json& j );

	/// Initializes shapes
	/// @param[in] j Json object describing the shapes
	void init_shapes( const nlohmann::json& j );

	/// Initializes scripts
	/// @param[in] j Json object describing scripts
	void init_scripts( const nlohmann::json& j );

	/// Initializes scenes
	/// @param[in] j Json object describing the scenes
	void initScenes( const nlohmann::json& j );

	/// Loads a buffer in the cache
	/// @param[in] i Index of the buffer
	auto loadBuffer( const size_t i );

	/// Directory path of the gltf file
	std::string mPath;

	/// List of buffers
	std::vector<Buffer> mBuffers;

	/// Cache of buffers
	std::map<const size_t, std::vector<char>> mBuffersCache;

	/// List of buffer views
	std::vector<BufferView> mBufferViews;

	/// List of cameras
	std::vector<Camera> cameras;

	/// List of samplers
	std::vector<Sampler> samplers;

	/// List of images
	std::vector<Image> images;

	/// List of textures
	std::vector<Texture> textures;

	/// List of accessors
	std::vector<Accessor> mAccessors;

	/// List of materials
	std::vector<Material> mMaterials;

	/// List of meshes
	std::vector<Mesh> meshes;

	/// List of lights
	std::vector<Light> lights;

	/// List of nodes
	std::vector<Node> nodes;

	/// List of animations
	std::vector<Animation> animations;

	/// List of shapes (abstract)
	std::vector<std::unique_ptr<Shape>> shapes;

	/// List of scripts
	std::vector<Script> scripts;

	/// List of scenes
	std::vector<Scene> mScenes;

	/// Current scene
	Scene* mScene = nullptr;
};


template <typename T>
T from_string( const std::string& s );

template <>
spot::gltf::Gltf::Accessor::Type from_string<spot::gltf::Gltf::Accessor::Type>( const std::string& s );

template <>
spot::gltf::Mesh::Primitive::Semantic from_string<spot::gltf::Mesh::Primitive::Semantic>( const std::string& s );

template <>
spot::gltf::Gltf::Animation::Sampler::Interpolation from_string<spot::gltf::Gltf::Animation::Sampler::Interpolation>(
    const std::string& i );

template <>
spot::gltf::Gltf::Animation::Target::Path from_string<spot::gltf::Gltf::Animation::Target::Path>( const std::string& p );

template <>
spot::gltf::Bounds::Type from_string<spot::gltf::Bounds::Type>( const std::string& b );

template <typename T>
std::string to_string( const T& t );

template <>
std::string to_string<spot::gltf::Gltf::Accessor::Type>( const spot::gltf::Gltf::Accessor::Type& t );

template <>
std::string to_string<spot::gltf::Mesh::Primitive::Semantic>( const spot::gltf::Mesh::Primitive::Semantic& s );

template <>
std::string to_string<spot::gltf::Sampler::Filter>( const spot::gltf::Sampler::Filter& f );

template <>
std::string to_string<spot::gltf::Sampler::Wrapping>( const spot::gltf::Sampler::Wrapping& w );

template <>
std::string to_string<spot::gltf::Mesh::Primitive::Mode>( const spot::gltf::Mesh::Primitive::Mode& m );

}  // namespace spot::gltf