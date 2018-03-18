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

	// Accessors
	initAccessors(j["accessors"]);
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
		accessor.componentType = a["componentType"].get<size_t>();

		// Count
		accessor.count = a["count"].get<size_t>();

		// Type
		accessor.type = a["type"].get<string>();

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
