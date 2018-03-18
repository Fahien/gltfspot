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

		// Buffer length in bytes
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
}


vector<Gltf::BufferView>& Gltf::GetBufferViews()
{
	return mBufferViews;
}
