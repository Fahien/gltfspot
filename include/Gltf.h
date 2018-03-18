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

	/// A buffer points to binary geometry, animation, or skins
	struct Buffer
	{
		/// Length of the buffer in bytes
		size_t byteLength;
		/// Uri of the buffer
		std::string uri;
	};

	/// A view into a buffer generally representing a subset of the buffer
	struct BufferView
	{
		/// Index of the buffer
		size_t buffer;
		/// Offset into the buffer in bytes
		size_t byteOffset;
		/// Length of the bufferView in bytes
		size_t byteLength;
		/// Stride, in bytes
		size_t byteStride;
		/// Target that the GPU buffer should be bound to
		size_t target;
	};

	/// A typed view into a bufferView
	struct Accessor
	{
		/// Index of the buffer view
		size_t bufferView;
		/// Offset relative to the start of the bufferView in bytes
		size_t byteOffset;
		/// Datatype of components in the attribute
		size_t componentType;
		/// Number of attributes referenced by this accessor
		size_t count;
		/// Specifies if the attribute is a scalar, vector, or matrix
		std::string type;
		/// Maximum value of each component in this attribute
		std::vector<float> max;
		/// Minimum value of each component in this attribute
		std::vector<float> min;
	};

	/// Constructs a Gltf object
	/// @param[in] path Gltf file path
	/// @param[in] j Json object describing the model
	Gltf(const std::string& path, const nlohmann::json& j);

	/// Loads a GLtf model from path
	/// @param[in] path Gltf file path
	static Gltf Load(const std::string& path);

	/// Returns the buffer number i
	/// @param[in] i Index of the buffer
	std::vector<char>& GetBuffer(const size_t i);

	/// Returns the buffer views
	std::vector<BufferView>& GetBufferViews();

	/// Returns the accessors
	std::vector<Accessor>& GetAccessors();

	/// glTF asset
	Asset asset;

  private:

	/// Initializes asset
	/// @param[in] j Json object describing the asset
	void initAsset(const nlohmann::json& j);

	/// Initializes buffers
	/// @param[in] j Json object describing the buffers
	void initBuffers(const nlohmann::json& j);

	/// Initializes bufferViews
	/// @param[in] j Json object describing the bufferViews
	void initBufferViews(const nlohmann::json& j);

	/// Initializes accessors
	/// @param[in] j Json object describing the accessors
	void initAccessors(const nlohmann::json& j);

	/// Loads a buffer in the cache
	/// @param[in] i Index of the buffer
	auto loadBuffer(const size_t i);

	/// Directory path of the gltf file
	std::string mPath;

	/// List of buffers
	std::vector<Buffer> mBuffers;

	/// Cache of buffers
	std::map<const size_t, std::vector<char>> mBuffersCache;

	/// List of buffer views
	std::vector<BufferView> mBufferViews;

	/// List of accessors
	std::vector<Accessor> mAccessors;
};

}
