#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>

#include "Gltf.h"

using namespace std;
using namespace gltfspot;
using namespace nlohmann;


void printJson(const string& path)
{
	ifstream i{ path };
	json j;
	i >> j;
	cout << j["asset"]["version"] << endl;
}


void printAsset(Gltf& model)
{
	cout << endl << "# Asset" << endl;
	cout << model.asset.version   << endl;
	cout << model.asset.generator << endl;
	cout << model.asset.copyright << endl;
}


void printBuffer(Gltf& model)
{
	cout << endl << "# Buffer" << endl;
	auto& buffer = model.GetBuffer(0);
	for (size_t i{ 0 }; i < buffer.size(); ++i)
	{
		if (i != 0 && (i % 32) == 0)
		{
			cout << endl;
		}
		auto& b = buffer[i];
		printf("%02X ", b & 0xff);
	}
	cout << endl;
}


void printBufferViews(Gltf& model)
{
	cout << endl << "# BufferViews" << endl;
	for (auto view : model.GetBufferViews())
	{
		cout << view.buffer << ", "
		     << view.byteLength << ", "
		     << view.target << endl;
	}
}


void printAccessors(Gltf& model)
{
	cout << endl << "# Accessors" << endl;
	for (auto& acc : model.GetAccessors())
	{
		cout << acc.count << ", ";
		for (auto v : acc.max)
		{
			cout << v << ", ";
		}
		for (auto v : acc.min)
		{
			cout << v << ", ";
		}
		std::cout << endl;
	}
}


void printMaterials(Gltf& model)
{
	cout << endl << "# Materials" << endl;
	for (auto& material : model.GetMaterials())
	{
		cout << material.name << endl;

		cout << "[ ";
		for (auto val : material.pbrMetallicRoughness.baseColorFactor)
		{
			cout << val << " ";
		}
		cout << "]" << endl;

		cout << "Metallic : " << material.pbrMetallicRoughness.metallicFactor << endl;
		cout << "Roughness: " << material.pbrMetallicRoughness.roughnessFactor << endl;
	}
}


void printMeshes(Gltf& model)
{
	cout << endl << "# Meshes" << endl;

	for (auto& mesh : model.GetMeshes())
	{
		cout << mesh.name << endl;

		for (auto& p : mesh.primitives)
		{
			cout << "  # Attributes" << endl;
			for (auto& a : p.attributes)
			{
				cout << "  " << static_cast<unsigned>(a.first) << ": " << a.second << endl;
			}

			cout << "indices["  << p.indices  << "] "
				 << "material[" << p.material << "] "
				 << "mode["     << p.mode     << "] "
				 << endl;
		}
	}
}


int main()
{
	// Json
	cout << "# Json" << endl;
	string modelPath{ "../test/model/box/Box.gltf" };
	printJson(modelPath);

	// Gltf
	Gltf model{ Gltf::Load(modelPath) };

	// Asset
	printAsset(model);

	// Buffer
	printBuffer(model);

	// BufferView
	printBufferViews(model);

	// Accessors
	printAccessors(model);

	// Materials
	printMaterials(model);

	// Meshes
	printMeshes(model);

	return EXIT_SUCCESS;
}
