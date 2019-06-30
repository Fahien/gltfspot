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

		/// Orthographic camera
		Ortographic orthographic;
		/// Perspective camera
		Perspective perspective;
		/// Specifies if the camera uses a perspective or orthographic projection
		std::string type;
		/// Name
		std::string name;
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
		std::string name;
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
		std::string name = "";
	};

	/// Texture and its sampler
	struct Texture
	{
		/// Sampler used by this texture
		Sampler* sampler = nullptr;
		/// Image used by this texture
		Image* source = nullptr;
		/// User-defined name of this object
		std::string name;
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
		std::string name;
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
			/// Index of the accessor that contains the indices
			unsigned indices;
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
		std::string name;
		/// extensions TODO Dictionary object with extension-specific objects
		/// extras TODO Application-specific data
		void* extras;
	};

	/// Node in the node hierarchy
	struct Node
	{
		/// Camera referenced by this node
		Camera* pCamera = nullptr;
		/// This node's children
		std::vector<Node*> children;
		// This node's children indices
		std::vector<unsigned> childrenIndices;
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
		std::string name;
	};

	/// Root nodes of a scene
	struct Scene
	{
		/// Indices of each root node
		std::vector<Node*> nodes;
		/// User-defined name of this object
		std::string name = "default";
	};

	/// Move contructs a Gltf object
	/// @param[in] g Gltf object
	Gltf( Gltf&& g ) = default;

	/// Move assign a Gltf object
	/// @param[in] g Gltf object
	Gltf& operator=( Gltf&& g ) = default;

	/// Constructs a Gltf object
	/// @param[in] j Json object describing the model
	/// @param[in] path Gltf file path
	Gltf( const nlohmann::json& j, const std::string& path = "." );

	// Delete copy constructor and assignment
	Gltf( const Gltf& ) = delete;
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

	/// @return Nodes
	std::vector<Node>& GetNodes();

	/// @return Scenes
	std::vector<Scene>& GetScenes();

	/// @return Current scene
	Scene* GetScene();

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

	/// Initializes nodes
	/// @param[in] j Json object describing the nodes
	void initNodes( const nlohmann::json& j );

	/// Initializes scenes
	/// @param[in] j Json object describing the scenes
	void initScenes( const nlohmann::json& j );

	/// Loads a buffer in the cache
	/// @param[in] i Index of the buffer
	auto loadBuffer( const size_t i );

	/// Directory path of the gltf file
	std::string mPath{};

	/// List of buffers
	std::vector<Buffer> mBuffers{};

	/// Cache of buffers
	std::map<const size_t, std::vector<char>> mBuffersCache{};

	/// List of buffer views
	std::vector<BufferView> mBufferViews{};

	/// List of cameras
	std::vector<Camera> mCameras;

	/// List of samplers
	std::vector<Sampler> mSamplers{};

	/// List of images
	std::vector<Image> mImages{};

	/// List of textures
	std::vector<Texture> mTextures{};

	/// List of accessors
	std::vector<Accessor> mAccessors{};

	/// List of materials
	std::vector<Material> mMaterials{};

	/// List of meshes
	std::vector<Mesh> mMeshes{};

	/// List of nodes
	std::vector<Node> mNodes{};

	/// List of scenes
	std::vector<Scene> mScenes{};

	/// Current scene
	Scene* mScene = nullptr;
};


}  // namespace gltfspot

template <typename T>
T from_string( const std::string& s );

template <typename T>
std::string to_string( const T& t );

template <>
gltfspot::Gltf::Accessor::Type from_string<gltfspot::Gltf::Accessor::Type>( const std::string& s );

template <>
std::string to_string<gltfspot::Gltf::Accessor::Type>( const gltfspot::Gltf::Accessor::Type& t );

template <>
gltfspot::Gltf::Mesh::Primitive::Semantic from_string<gltfspot::Gltf::Mesh::Primitive::Semantic>( const std::string& s );

template <>
std::string to_string<gltfspot::Gltf::Mesh::Primitive::Semantic>( const gltfspot::Gltf::Mesh::Primitive::Semantic& s );

template <>
std::string to_string<gltfspot::Gltf::Sampler::Filter>( const gltfspot::Gltf::Sampler::Filter& f );

template <>
std::string to_string<gltfspot::Gltf::Sampler::Wrapping>( const gltfspot::Gltf::Sampler::Wrapping& w );

template <>
std::string to_string<gltfspot::Gltf::Mesh::Primitive::Mode>( const gltfspot::Gltf::Mesh::Primitive::Mode& m );

#endif  // GST_GLTF_H_
