#ifndef GST_GLTF_H_
#define GST_GLTF_H_

#include <mathspot/Math.h>

#include "nlohmann/json.hpp"

namespace gltfspot
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
			ARRAY_BUFFER         = 34962,
			ELEMENT_ARRAY_BUFFER = 34963
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
		Target target = Target::ARRAY_BUFFER;
	};

	/// Camera's projection.
	/// A node can reference a camera to apply a transform to place the camera in the scene
	struct Camera
	{
		/// Orthographic Camera
		struct Ortographic
		{
			/// Horizontal magnification
			float xmag = 0.0f;
			/// Vertical magnification
			float ymag = 0.0f;
			/// Distance to the far clipping plane
			float zfar = 0.0f;
			/// Distance to the near clipping plane
			float znear = 0.0f;
		};

		/// Perspective camera
		struct Perspective
		{
			/// Aspect ratio of the field of view
			float aspectRatio = 1.0f;
			/// Vertical field of view in radians
			float yfov = 0.0f;
			/// Distance to the far clipping plane
			float zfar = 0.0f;
			/// Distance to the near clipping plane
			float znear = 0.0f;
		};

		/// Type of the camera
		enum class Type
		{
			Ortographic,
			Perspective
		};

		/// Orthographic camera
		Ortographic orthographic;
		/// Perspective camera
		Perspective perspective;
		/// Specifies if the camera uses a perspective or orthographic projection
		Type type;
		/// Name
		std::string name = "Unknown";
	};

	/// Texture sampler properties for filtering and wrapping modes
	struct Sampler
	{
		/// Magnification/Minification filter
		enum class Filter
		{
			NONE                   = 0,
			NEAREST                = 9728,
			LINEAR                 = 9729,
			NEAREST_MIPMAP_NEAREST = 9984,
			LINEAR_MIPMAP_NEAREST  = 9985,
			NEAREST_MIPMAP_LINEAR  = 9986,
			LINEAR_MIPMAP_LINEAR   = 9987
		};

		/// Wrapping mode
		enum class Wrapping
		{
			CLAMP_TO_EDGE   = 33071,
			MIRRORED_REPEAT = 33648,
			REPEAT          = 10497
		};

		/// Magnification filter
		Filter magFilter = Filter::NONE;
		/// Minification filter
		Filter minFilter = Filter::NONE;
		/// S wrapping mode
		Wrapping wrapS = Wrapping::REPEAT;
		/// T wrapping mode
		Wrapping wrapT = Wrapping::REPEAT;
		/// User-defined name of this object
		std::string name = "Unknown";
	};

	/// Image data used to create a texture
	struct Image
	{
		/// Uri of the image
		std::string uri = "";

		/// MIME type
		std::string mime_type = "";

		/// Buffer view index
		uint32_t buffer_view = 0;

		/// Name
		std::string name = "Unknown";
	};

	/// Texture and its sampler
	struct Texture
	{
		/// Sampler used by this texture
		Sampler* sampler = nullptr;
		/// Image used by this texture
		Image* source = nullptr;
		/// User-defined name of this object
		std::string name = "Unknown";
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

	/// Material appearance of a primitive
	struct Material
	{
		/// Metallic-Roughness Material
		struct PbrMetallicRoughness
		{
			/// Base color of the material
			std::vector<float> baseColorFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
			/// Base color texture
			Texture* baseColorTexture = nullptr;
			/// Metalness of the material
			float metallicFactor = 1.0f;
			/// Roughness of the material
			float roughnessFactor = 1.0f;
		};

		/// User-defined name of this object
		std::string name = "Unknown";
		/// Metallic-Roughness Material
		PbrMetallicRoughness pbrMetallicRoughness;
	};

	/// Set of primitives to be rendered
	struct Mesh
	{
		/// Geometry to be rendered with the given material
		struct Primitive
		{
			/// Enumerated value identifying the vertex attribute
			enum class Semantic
			{
				NONE,
				POSITION,
				NORMAL,
				TANGENT,
				TEXCOORD_0,
				TEXCOORD_1,
				COLOR_0,
				JOINTS_0,
				WEIGHTS_0
			};

			/// Type of primitives to render
			enum class Mode
			{
				POINTS,
				LINES,
				LINE_LOOP,
				LINE_STRIP,
				TRIANGLES,
				TRIANGLE_STRIP,
				TRIANGLE_FAN
			};

			/// Dictionary object, where each key corresponds to mesh attribute semantic and
			/// each value is the index of the accessor containing attribute's data (required)
			std::map<Semantic, unsigned> attributes;
			/// If >= 0, it is the index of the accessor that contains the indices
			int32_t indices = -1;
			/// Index of the material to apply to this primitive when rendering
			unsigned material;
			/// Type of primitives to render
			Mode mode = Mode::TRIANGLES;
			/// targets TODO An array of Morph Targets, each Morph Target is a dictionary mapping attributes (only POSITION,
			/// NORMAL, and TANGENT supported) to their deviations in the Morph Target extensions TODO Dictionary object with
			/// extension-specific objects Application-specific data
			void* extras;
		};

		/// Array of primitives, each defining geometry to be rendered with a material (required)
		std::vector<Primitive> primitives;
		/// Array of weights to be applied to the Morph Targets
		std::vector<float> weights;
		/// User-defined name of this object
		std::string name = "Unknown";
		/// extensions TODO Dictionary object with extension-specific objects
		/// extras TODO Application-specific data
		void* extras;
	};

	/// Punctual light that emit light in well-defined directions and intensities
	struct Light
	{
		/// Type of light
		enum class Type
		{
			Point,
			Directional,
			Spot
		};

		/// Spot light which emits light in a cone in the direction of the local -z axis
		struct Spot
		{
			/// Angle in radians, from center where falloff begins
			float inner_cone_angle = 0.0f;
			/// Angle in radians, from center where falloff ends
			float outer_cone_angle = mathspot::kPi / 4.0f;
		};

		/// Name of the light
		std::string name = "Unknown";
		/// Color of the light
		mathspot::Vec3 color = mathspot::Vec3{ 1.0f, 1.0f, 1.0f };
		/// Unit depending on the type of light
		/// Point and spot lights use luminous intensity in candela (lm/sr)
		/// Directional lights use illuminance in lux (lm/m^2)
		float intensity = 1.0f;
		/// Type of light
		Type type = Type::Point;
		/// Distance cutoff where intensity reaches zero (point and spot)
		/// If <= 0 it is assumed to be infinite
		float range = 0.0f;
		/// Spot light values
		Spot spot = {};
	};

	/// Node in the node hierarchy
	struct Node
	{
		/// Gltf owning the node
		Gltf* gltf = nullptr;
		/// Index of this node within the nodes vector
		size_t index = 0;
		/// Parent of this node
		Node* parent = nullptr;
		/// Camera referenced by this node
		Camera* pCamera = nullptr;
		/// This node's children
		std::vector<Node*> children;
		// This node's children indices
		std::vector<size_t> children_indices;
		/// Floating-point 4x4 transformation matrix stored in column-major order
		mathspot::Mat4 matrix = mathspot::Mat4::identity;
		/// Mesh in this node
		Mesh* pMesh = nullptr;
		/// Unit quaternion rotation in the order (x, y, z, w), where w is the scalar
		mathspot::Quat rotation = mathspot::Quat::identity;
		/// Non-uniform scale
		mathspot::Vec3 scale = mathspot::Vec3{ 1.0f, 1.0f, 1.0f };
		/// Translation
		mathspot::Vec3 translation = mathspot::Vec3{ 0.0f, 0.0f, 0.0f };
		/// User-defined name of this object
		std::string name = "Unknown";
		/// If not negative, index of light contained by this node
		int32_t light_index = -1;
		/// Light referenced by this node
		Light* light = nullptr;

		/// @return A newly created Node as a child of this
		/// @param[in] name Name of the node
		Node& create_child( const std::string& name );

		void remove_from_parent();
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
	std::vector<Camera>& GetCameras() { return mCameras; }

	/// @return Samplers
	std::vector<Sampler>& GetSamplers();

	/// @return Images
	std::vector<Image>& GetImages();

	/// @return Textures
	std::vector<Texture>& GetTextures();

	/// @return Accessors
	std::vector<Accessor>& GetAccessors();

	/// @return Materials
	std::vector<Material>& GetMaterials();

	/// @return Meshes
	std::vector<Mesh>& GetMeshes();

	/// @return Lights
	std::vector<Light>& get_lights();

	/// @return Nodes
	std::vector<Node>& GetNodes();

	/// @return Animations
	std::vector<Animation>& get_animations();

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
	void initMeshes( const nlohmann::json& j );

	/// Initializes lights
	/// @param[in] j Json object describing the lights
	void init_lights( const nlohmann::json& j );

	/// Initializes nodes
	/// @param[in] j Json object describing the nodes
	void initNodes( const nlohmann::json& j );

	/// Initializes animations
	/// @param[in] j Json object describing the animations
	void init_animations( const nlohmann::json& j );

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
	std::vector<Camera> mCameras;

	/// List of samplers
	std::vector<Sampler> mSamplers;

	/// List of images
	std::vector<Image> mImages;

	/// List of textures
	std::vector<Texture> mTextures;

	/// List of accessors
	std::vector<Accessor> mAccessors;

	/// List of materials
	std::vector<Material> mMaterials;

	/// List of meshes
	std::vector<Mesh> mMeshes;

	/// List of lights
	std::vector<Light> lights;

	/// List of nodes
	std::vector<Node> mNodes;

	/// List of animations
	std::vector<Animation> animations;

	/// List of scenes
	std::vector<Scene> mScenes;

	/// Current scene
	Scene* mScene = nullptr;
};


template <typename T>
T from_string( const std::string& s );

template <>
gltfspot::Gltf::Accessor::Type from_string<gltfspot::Gltf::Accessor::Type>( const std::string& s );

template <>
gltfspot::Gltf::Mesh::Primitive::Semantic from_string<gltfspot::Gltf::Mesh::Primitive::Semantic>( const std::string& s );

template <>
gltfspot::Gltf::Animation::Sampler::Interpolation from_string<gltfspot::Gltf::Animation::Sampler::Interpolation>(
    const std::string& i );

template <>
gltfspot::Gltf::Animation::Target::Path from_string<gltfspot::Gltf::Animation::Target::Path>( const std::string& p );

template <typename T>
std::string to_string( const T& t );

template <>
std::string to_string<gltfspot::Gltf::Accessor::Type>( const gltfspot::Gltf::Accessor::Type& t );

template <>
std::string to_string<gltfspot::Gltf::Mesh::Primitive::Semantic>( const gltfspot::Gltf::Mesh::Primitive::Semantic& s );

template <>
std::string to_string<gltfspot::Gltf::Sampler::Filter>( const gltfspot::Gltf::Sampler::Filter& f );

template <>
std::string to_string<gltfspot::Gltf::Sampler::Wrapping>( const gltfspot::Gltf::Sampler::Wrapping& w );

template <>
std::string to_string<gltfspot::Gltf::Mesh::Primitive::Mode>( const gltfspot::Gltf::Mesh::Primitive::Mode& m );

}  // namespace gltfspot

#endif  // GST_GLTF_H_
