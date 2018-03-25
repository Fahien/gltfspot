#include <fstream>
#include <stdexcept>

#include "Gltf.h"

using namespace std;
using namespace gltfspot;
using namespace nlohmann;


template<typename T>
T from_string(const string& s);


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
			view.target = v["target"].get<size_t>();
		}

		mBufferViews.push_back(move(view));
	}
}


template<>
Gltf::Accessor::Type from_string<Gltf::Accessor::Type>(const std::string& s)
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
