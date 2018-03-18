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
		/// The glTF version that this asset targets
		std::string version;
		/// Tool that generated this glTF model. Useful for debugging
		std::string generator;
		/// A copyright message suitable for display to credit the content creator
		std::string copyright;
	};

	/// A buffer points to binary geometry, animation, or skins
	struct Buffer
	{
		/// The length of the buffer in bytes
		size_t byteLength;
		/// The uri of the buffer
		std::string uri;
	};

	/// A view into a buffer generally representing a subset of the buffer
	struct BufferView
	{
		/// The index of the buffer
		size_t buffer;
		/// The offset into the buffer in bytes
		size_t byteOffset;
		/// The length of the bufferView in bytes
		size_t byteLength;
		/// The stride, in bytes
		size_t byteStride;
		/// The target that the GPU buffer should be bound to
		size_t target;
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
};

}
