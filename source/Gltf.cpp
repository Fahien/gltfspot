#include <fstream>
#include <stdexcept>

#include "Gltf.h"

using namespace std;
using namespace gltfspot;
using namespace nlohmann;



Gltf::Gltf(const string& path, const json& j)
{
	// Get the directory path
	auto index = path.find_last_of("/\\");
	mPath = path.substr(0, index);

	// Asset
	initAsset(j["asset"]);

	// Buffer
	if (j.count("buffers"))
	{
		initBuffers(j["buffers"]);
	}

	// BufferViews
	if (j.count("bufferViews"))
	{
		initBufferViews(j["bufferViews"]);
	}

	// Samplers
	if (j.count("samplers"))
	{
		initSamplers(j["samplers"]);
	}

	// Images
	if (j.count("images"))
	{
		initImages(j["images"]);
	}

	// Accessors
	initAccessors(j["accessors"]);

	// Materials
	if (j.count("materials"))
	{
		initMaterials(j["materials"]);
	}

	// Meshes
	if (j.count("meshes"))
	{
		initMeshes(j["meshes"]);
	}

	// Nodes
	if (j.count("nodes"))
	{
		initNodes(j["nodes"]);
	}

	// Scenes
	if (j.count("scenes"))
	{
		initScenes(j["scenes"]);

		auto uIndex = j["scene"].get<uint64_t>();
		mScene = &mScenes[uIndex];
	}
}


void Gltf::initAsset(const json& j)
{
	// Version (mandatory)
	asset.version = j["version"].get<string>();
	
	// Generator
	if (j.count("generator"))
	{
		asset.generator = j["generator"].get<string>();
	}

	// Copyright
	if (j.count("copyright"))
	{
		asset.copyright = j["copyright"].get<string>();
	}
}


void Gltf::initBuffers(const json& j)
{
	for (const auto& b : j)
	{
		Buffer buffer;

		// Buffer length in bytes (mandatory)
		buffer.byteLength = b["byteLength"].get<size_t>();

		// Uri of the binary file to upload
		if (b.count("uri"))
		{
			buffer.uri = mPath + "/" + b["uri"].get<string>();
		}

		mBuffers.push_back(buffer);
	}
}


void Gltf::initBufferViews(const json& j)
{
	for (const auto& v : j)
	{
		BufferView view;

		// Buffer
		view.buffer = v["buffer"].get<size_t>();

		// Byte offset
		if (v.count("byteOffset"))
		{
			view.byteOffset = v["byteOffset"].get<size_t>();
		}

		// Byte length
		if (v.count("byteLength"))
		{
			view.byteLength = v["byteLength"].get<size_t>();
		}

		// Byte stride
		if (v.count("byteStride"))
		{
			view.byteStride = v["byteStride"].get<size_t>();
		}

		// Target
		if (v.count("target"))
		{
			view.target = static_cast<Gltf::BufferView::Target>(v["target"].get<size_t>());
		}

		mBufferViews.push_back(move(view));
	}
}


template<>
string to_string<Gltf::Sampler::Filter>(const Gltf::Sampler::Filter& f)
{
	switch (f)
	{
		case Gltf::Sampler::Filter::NONE:    return "NONE";
		case Gltf::Sampler::Filter::NEAREST: return "NEAREST";
		case Gltf::Sampler::Filter::LINEAR:  return "LINEAR";
		case Gltf::Sampler::Filter::NEAREST_MIPMAP_NEAREST: return "NEAREST_MIPMAP_NEAREST";
		case Gltf::Sampler::Filter::LINEAR_MIPMAP_NEAREST:  return "LINEAR_MIPMAP_NEAREST";
		case Gltf::Sampler::Filter::NEAREST_MIPMAP_LINEAR:  return "NEAREST_MIPMAP_LINEAR";
		case Gltf::Sampler::Filter::LINEAR_MIPMAP_LINEAR:   return "LINEAR_MIPMAP_LINEAR";
	}
}


template<>
string to_string<Gltf::Sampler::Wrapping>(const Gltf::Sampler::Wrapping& w)
{
	switch (w)
	{
		case Gltf::Sampler::Wrapping::CLAMP_TO_EDGE:   return "CLAMP_TO_EDGE";
		case Gltf::Sampler::Wrapping::MIRRORED_REPEAT: return "MIRRORED_REPEAT";
		case Gltf::Sampler::Wrapping::REPEAT:          return "REPEAT";
	}
}


void Gltf::initSamplers(const json& j)
{
	for (const auto& s : j)
	{
		Sampler sampler;

		// Mag Filter
		if (s.count("magFilter"))
		{
			sampler.magFilter = static_cast<Sampler::Filter>(s["magFilter"].get<int>());
		}

		// Min Filter
		if (s.count("minFilter"))
		{
			sampler.minFilter = static_cast<Sampler::Filter>(s["minFilter"].get<int>());
		}

		// WrapS
		if (s.count("wrapS"))
		{
			sampler.wrapS = static_cast<Sampler::Wrapping>(s["wrapS"].get<int>());
		}

		// WrapT
		if (s.count("wrapT"))
		{
			sampler.wrapT = static_cast<Sampler::Wrapping>(s["wrapT"].get<int>());
		}

		// Name
		if (s.count("name"))
		{
			sampler.name = s["name"].get<string>();
		}

		mSamplers.push_back(sampler);
	}
}


void Gltf::initImages(const json& j)
{
	for (const auto& i : j)
	{
		Image image;

		if (i.count("uri"))
		{
			image.uri = i["uri"].get<string>();
		}

		mImages.push_back(image);
	}
}


template<>
Gltf::Accessor::Type from_string<Gltf::Accessor::Type>(const string& s)
{
	if (s == "SCALAR")
	{
		return Gltf::Accessor::Type::SCALAR;
	}
	else if (s == "VEC2")
	{
		return Gltf::Accessor::Type::VEC2;
	}
	else if (s == "VEC3")
	{
		return Gltf::Accessor::Type::VEC3;
	}
	else if (s == "VEC4")
	{
		return Gltf::Accessor::Type::VEC4;
	}
	else if (s == "MAT2")
	{
		return Gltf::Accessor::Type::MAT2;
	}
	else if (s == "MAT3")
	{
		return Gltf::Accessor::Type::MAT3;
	}
	else if (s == "MAT4")
	{
		return Gltf::Accessor::Type::MAT4;
	}
	else
	{
		assert(true);
		return Gltf::Accessor::Type::NONE;
	}
}


template<>
std::string to_string<Gltf::Accessor::Type>(const Gltf::Accessor::Type& t)
{
	if (t == Gltf::Accessor::Type::SCALAR)
	{
		return "SCALAR";
	}
	else if (t == Gltf::Accessor::Type::VEC2)
	{
		return "VEC2";
	}
	else if (t == Gltf::Accessor::Type::VEC3)
	{
		return "VEC3";
	}
	else if (t == Gltf::Accessor::Type::VEC4)
	{
		return "VEC4";
	}
	else if (t == Gltf::Accessor::Type::MAT2)
	{
		return "MAT2";
	}
	else if (t == Gltf::Accessor::Type::MAT3)
	{
		return "MAT3";
	}
	else if (t == Gltf::Accessor::Type::MAT4)
	{
		return "MAT4";
	}
	else
	{
		assert(true);
		return "NONE";
	}
}


void Gltf::initAccessors(const json& j)
{
	for (const auto& a : j)
	{
		Accessor accessor;

		// Buffer view
		if (a.count("bufferView"))
		{
			accessor.bufferView = a["bufferView"].get<size_t>();
		}

		// Byte offset
		if (a.count("byteOffset"))
		{
			accessor.byteOffset = a["byteOffset"].get<size_t>();
		}

		// Component type
		accessor.componentType = a["componentType"].get<Accessor::ComponentType>();

		// Count
		accessor.count = a["count"].get<size_t>();

		// Type
		accessor.type = from_string<Gltf::Accessor::Type>(a["type"].get<string>());

		// Max
		if (a.count("max"))
		{
			for (const auto& value : a["max"])
			{
				accessor.max.push_back(value.get<float>());
			}
		}

		// Min
		if (a.count("min"))
		{
			for (const auto& value : a["min"])
			{
				accessor.min.push_back(value.get<float>());
			}
		}

		mAccessors.push_back(accessor);
	}
}


void Gltf::initMaterials(const json& j)
{
	for (const auto& m : j)
	{
		Gltf::Material material;

		// Name
		if (m.count("name"))
		{
			material.name = m["name"].get<string>();
		}

		// PbrMetallicRoughness
		if (m.count("pbrMetallicRoughness"))
		{
			auto& mr = m["pbrMetallicRoughness"];

			if (mr.count("baseColorFactor"))
			{
				material.pbrMetallicRoughness.baseColorFactor = mr["baseColorFactor"].get<vector<float>>();
			}

			if (mr.count("metallicFactor"))
			{
				material.pbrMetallicRoughness.metallicFactor = mr["metallicFactor"].get<float>();
			}

			if (mr.count("roughnessFactor"))
			{
				material.pbrMetallicRoughness.roughnessFactor = mr["roughnessFactor"].get<float>();
			}
		}

		mMaterials.push_back(material);
	}
}


template<>
Gltf::Mesh::Primitive::Semantic from_string<Gltf::Mesh::Primitive::Semantic>(const std::string& s)
{
	if (s == "POSITION")
	{
		return Gltf::Mesh::Primitive::Semantic::POSITION;
	}
	else if (s == "NORMAL")
	{
		return Gltf::Mesh::Primitive::Semantic::NORMAL;
	}
	else if (s == "TANGENT")
	{
		return Gltf::Mesh::Primitive::Semantic::TANGENT;
	}
	else if (s == "TEXCOORD_0")
	{
		return Gltf::Mesh::Primitive::Semantic::TEXCOORD_0;
	}
	else if (s == "TEXCOORD_1")
	{
		return Gltf::Mesh::Primitive::Semantic::TEXCOORD_1;
	}
	else if (s == "COLOR_0")
	{
		return Gltf::Mesh::Primitive::Semantic::COLOR_0;
	}
	else if (s == "JOINTS_0")
	{
		return Gltf::Mesh::Primitive::Semantic::JOINTS_0;
	}
	else if (s == "WEIGHTS_0")
	{
		return Gltf::Mesh::Primitive::Semantic::WEIGHTS_0;
	}
	else
	{
		assert(true);
		return Gltf::Mesh::Primitive::Semantic::NONE;
	}
}


template<>
std::string to_string<Gltf::Mesh::Primitive::Semantic>(const Gltf::Mesh::Primitive::Semantic& s)
{
	if (s == Gltf::Mesh::Primitive::Semantic::POSITION)
	{
		return "POSITION";
	}
	else if (s == Gltf::Mesh::Primitive::Semantic::NORMAL)
	{
		return "NORMAL";
	}
	else if (s == Gltf::Mesh::Primitive::Semantic::TANGENT)
	{
		return "TANGENT";
	}
	else if (s == Gltf::Mesh::Primitive::Semantic::TEXCOORD_0)
	{
		return "TEXCOORD_0";
	}
	else if (s == Gltf::Mesh::Primitive::Semantic::TEXCOORD_1)
	{
		return "TEXCOORD_1";
	}
	else if (s == Gltf::Mesh::Primitive::Semantic::COLOR_0)
	{
		return "COLOR_0";
	}
	else if (s == Gltf::Mesh::Primitive::Semantic::JOINTS_0)
	{
		return "JOINTS_0";
	}
	else if (s == Gltf::Mesh::Primitive::Semantic::WEIGHTS_0)
	{
		return "WEIGHTS_0";
	}
	else
	{
		assert(true);
		return "NONE";
	}
}


void Gltf::initMeshes(const json& j)
{
	for (const auto& m : j)
	{
		Gltf::Mesh mesh;

		// Name
		if (m.count("name"))
		{
			mesh.name = m["name"].get<string>();
		}

		// Primitives
		for (const auto& p : m["primitives"])
		{
			Gltf::Mesh::Primitive primitive;

			auto attributes = p["attributes"].get<map<string, unsigned>>();

			for (const auto& a : attributes)
			{
				auto semantic = from_string<Gltf::Mesh::Primitive::Semantic>(a.first);
				primitive.attributes.emplace(semantic, a.second);
			}

			if (p.count("indices"))
			{
				primitive.indices = p["indices"].get<unsigned>();
			}

			if (p.count("material"))
			{
				primitive.material = p["material"].get<unsigned>();
			}

			if (p.count("mode"))
			{
				primitive.mode = p["mode"].get<unsigned>();
			}

			mesh.primitives.push_back(primitive);
		}

		mMeshes.push_back(mesh);
	}
}


void Gltf::initNodes(const json& j)
{
	for (const auto& n : j)
	{
		Gltf::Node node;

		// Name
		if (n.count("name"))
		{
			node.name = n["name"].get<string>();
		}

		// Children
		if (n.count("children"))
		{
			node.children = n["children"].get<vector<unsigned>>();
		}

		// Matrix
		if (n.count("matrix"))
		{
			// TODO Improve this
			auto mvec = n["matrix"].get<vector<float>>();
			array<float, 16> marr;
			for (unsigned i{ 0 }; i < 16; ++i)
			{
				marr[i] = mvec[i];
			}
			node.matrix = mathspot::Mat4{ marr.data() };
		}

		// Mesh
		if (n.count("mesh"))
		{
			unsigned m = n["mesh"];
			node.pMesh = &mMeshes[m];
		}

		mNodes.push_back(node);
	}
}


void Gltf::initScenes(const json& j)
{
	for (const auto& s : j)
	{
		Gltf::Scene scene;

		// Name
		if (s.count("name"))
		{
			scene.name = s["name"].get<string>();
		}

		// Nodes
		if (s.count("nodes"))
		{
			scene.nodes = s["nodes"].get<vector<uint64_t>>();
		}

		mScenes.push_back(scene);
	}
}


auto Gltf::loadBuffer(const size_t i)
{
	Buffer& b{ mBuffers[i] };

	vector<char> buffer(b.byteLength);

	ifstream file{ b.uri, ios::binary };
	if (!file.read(buffer.data(), b.byteLength))
	{
		throw runtime_error{ "Could not read uri" };
	}

	return mBuffersCache.emplace(i, move(buffer));
}


Gltf Gltf::Load(const string& path)
{
	// read a JSON file
	ifstream i{ path };
	json j;
	i >> j;
	Gltf model{ path, j };
	return model;
}


vector<char>& Gltf::GetBuffer(const size_t i)
{
	if (mBuffersCache.count(i))
	{
		return mBuffersCache[i];
	}

	auto pair = loadBuffer(i); // (iterator, bool)
	if (pair.second) // success
	{
		// (key, value)
		return (pair.first->second);
	}

	throw out_of_range{ "Could not find the buffer" };
}


vector<Gltf::BufferView>& Gltf::GetBufferViews()
{
	return mBufferViews;
}


vector<Gltf::Sampler>& Gltf::GetSamplers()
{
	return mSamplers;
}


vector<Gltf::Image>& Gltf::GetImages()
{
	return mImages;
}


vector<Gltf::Accessor>& Gltf::GetAccessors()
{
	return mAccessors;
}


vector<Gltf::Material>& Gltf::GetMaterials()
{
	return mMaterials;
}

vector<Gltf::Mesh>& Gltf::GetMeshes()
{
	return mMeshes;
}

vector<Gltf::Node>& Gltf::GetNodes()
{
	return mNodes;
}

vector<Gltf::Scene>& Gltf::GetScenes()
{
	return mScenes;
}

Gltf::Scene* Gltf::GetScene()
{
	return mScene;
}
