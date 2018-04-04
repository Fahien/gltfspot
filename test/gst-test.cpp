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
		cout << view.buffer     << ", "
			 << view.byteLength << ", ";
		
		switch(view.target)
		{
			case Gltf::BufferView::Target::NONE:
			{
				cout << "NONE" << endl;
				break;
			}
			case Gltf::BufferView::Target::ARRAY_BUFFER:
			{
				cout << "ARRAY_BUFFER" << endl;
				break;
			}
			case Gltf::BufferView::Target::ELEMENT_ARRAY_BUFFER:
			{
				cout << "ELEMENT_ARRAY_BUFFER" << endl;
				break;
			}
			default:
			{
				cerr << "Invalid BufferView::Target " << static_cast<size_t>(view.target) << endl;
				assert(true);
			}
		}
	}
}


void printSamplers(Gltf& model)
{
	cout << endl << "# Samplers" << endl;

	for (auto& sampler : model.GetSamplers())
	{
		cout << "MinFilter[" << to_string(sampler.minFilter) << "] "
		     << "MagFiler["  << to_string(sampler.magFilter) << "] "
		     << "WrapS["     << to_string(sampler.wrapS)     << "] "
		     << "WrapT["     << to_string(sampler.wrapT)     << "]" << endl;
	}
}


void printImages(Gltf& model)
{
	cout << endl << "# Images" << endl;

	for (auto& image : model.GetImages())
	{
		cout << "Uri[" << image.uri << "]" << endl;
	}
}


void printTextures(Gltf& model)
{
	cout << endl << "# Textures" << endl;

	for (auto& texture : model.GetTextures())
	{
		cout << "Sampler[..." << to_string(texture.sampler->minFilter) << "] "
		     << "Source[..." << texture.source->uri << "]" << endl;
	}
}


void printAccessors(Gltf& model)
{
	cout << endl << "# Accessors" << endl;
	for (auto& acc : model.GetAccessors())
	{
		cout << to_string(acc.type) << ", "
		     << acc.count           << ", ";
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
				cout << "  " << to_string(a.first) << ": " << a.second << endl;
			}

			cout << "indices["  << p.indices  << "] "
				 << "material[" << p.material << "] "
				 << "mode["     << p.mode     << "] "
				 << endl;
		}
	}
}


void printScenes(Gltf& model)
{
	cout << endl << "# Scenes" << endl;

	for (const auto& s : model.GetScenes())
	{
		cout << s.name << endl;

		cout << "nodes[ ";
		for (const auto& n : s.nodes)
		{
			cout << n << " ";
		}
		cout << "]" << endl;
	}

	cout << endl << "# Scene[" << model.GetScene()->name << "]" << endl;
}


void printNodes(Gltf& model)
{
	cout << endl << "# Nodes" << endl;

	for (const auto& node : model.GetNodes())
	{
		cout << node.name << endl;

		cout << "children[ ";
		for (const auto& v : node.children)
		{
			cout << v << " ";
		}
		cout << "]" << endl;

		cout << "matrix[ ";
		
		for (const auto& v : node.matrix.matrix)
		{
			cout << v << " ";
		}
		cout << "]" << endl;
	}
}


int main()
{
	// Json
	cout << "# Json" << endl;
	string modelPath{ "../test/model/BoxTextured/glTF/BoxTextured.gltf" };
	printJson(modelPath);

	// Gltf
	Gltf model{ Gltf::Load(modelPath) };

	// Asset
	printAsset(model);

	// Buffer
	printBuffer(model);

	// BufferView
	printBufferViews(model);

	// Samplers
	printSamplers(model);

	// Images
	printImages(model);

	// Textures
	printTextures(model);

	// Accessors
	printAccessors(model);

	// Materials
	printMaterials(model);

	// Meshes
	printMeshes(model);

	// Nodes
	printNodes(model);

	// Schenes
	printScenes(model);

	return EXIT_SUCCESS;
}
