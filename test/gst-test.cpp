#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <spot/gltf/gltf.h>

using namespace std;
using namespace spot::gltf;
using namespace nlohmann;


void printJson( const string& path )
{
	ifstream i{ path };
	json     j;
	i >> j;
	if ( j.count( "asset" ) )
	{
		cout << j["asset"]["version"] << endl;
	}
}


void printAsset( Gltf& model )
{
	cout << endl << "# Asset" << endl;
	cout << model.asset.version << endl;
	cout << model.asset.generator << endl;
	cout << model.asset.copyright << endl;
}


void printBuffer( Gltf& model )
{
	cout << endl << "# Buffer" << endl;
	auto& buffer = model.get_buffer( 0 );
	for ( size_t i{ 0 }; i < buffer.data.size(); ++i )
	{
		if ( i != 0 && ( i % 32 ) == 0 )
		{
			cout << endl;
		}
		auto& b = buffer.data[i];
		printf( "%02X ", b & 0xff );
	}
	cout << endl;
}


void printBufferViews( Gltf& model )
{
	cout << endl << "# BufferViews" << endl;
	for ( auto view : model.buffer_views )
	{
		cout << view.buffer_index << ", " << view.byte_length << ", ";

		switch ( view.target )
		{
			case BufferView::Target::ArrayBuffer:
			{
				cout << "ARRAY_BUFFER" << endl;
				break;
			}
			case BufferView::Target::ElementArrayBuffer:
			{
				cout << "ELEMENT_ARRAY_BUFFER" << endl;
				break;
			}
			default:
			{
				cout << "NONE" << endl;
			}
		}
	}
}


void printCameras( Gltf& model )
{
	cout << endl << "# Cameras" << endl;

	for ( auto& camera : model.cameras )
	{
		switch ( camera.type )
		{
			case Camera::Type::Ortographic:
			{
				cout << "orthographic" << endl;
				break;
			}
			case Camera::Type::Perspective:
			{
				cout << "perspective" << endl;
				break;
			}
			default:
			{
				cerr << "Invalid Camera::Type " << static_cast<uint32_t>( camera.type ) << endl;
				assert( false );
			}
		}
	}
}


void printSamplers( Gltf& model )
{
	cout << endl << "# Samplers" << endl;

	for ( auto& sampler : model.samplers )
	{
		cout << "MinFilter[" << to_string( sampler.minFilter ) << "] "
		     << "MagFiler[" << to_string( sampler.magFilter ) << "] "
		     << "WrapS[" << to_string( sampler.wrapS ) << "] "
		     << "WrapT[" << to_string( sampler.wrapT ) << "]" << endl;
	}
}


void printImages( Gltf& model )
{
	cout << endl << "# Images" << endl;

	for ( auto& image : model.images )
	{
		cout << "Uri[" << image.uri << "]" << endl;
	}
}


void printTextures( Gltf& model )
{
	cout << endl << "# Textures" << endl;

	for ( auto& texture : model.textures )
	{
		cout << "Sampler[..." << to_string( texture.sampler->minFilter ) << "] "
		     << "Source[..." << texture.get_source()->uri << "]" << endl;
	}
}


void printAccessors( Gltf& model )
{
	cout << endl << "# Accessors" << endl;
	for ( auto& acc : model.accessors )
	{
		cout << to_string( acc.type ) << ", " << acc.count << ", ";
		for ( auto v : acc.max )
		{
			cout << v << ", ";
		}
		for ( auto v : acc.min )
		{
			cout << v << ", ";
		}
		std::cout << endl;
	}
}


void printMaterials( Gltf& model )
{
	cout << endl << "# Materials" << endl;
	for ( auto& material : model.materials )
	{
		cout << material.name << endl;

		cout << "[ ";
		for ( auto val : material.pbr.base_color_factor )
		{
			cout << val << " ";
		}
		cout << "]" << endl;

		if ( auto texture = material.get_texture() )
		{
			cout << "Texture  :" << texture->get_source()->uri << endl;
		}

		cout << "Metallic :" << material.pbr.metallic_factor << endl;
		cout << "Roughness:" << material.pbr.roughness_factor << endl;
	}
}


void printMeshes( Gltf& model )
{
	cout << endl << "# Meshes" << endl;

	for ( auto& mesh : model.meshes )
	{
		cout << mesh.name << endl;

		for ( auto& p : mesh.primitives )
		{
			cout << "  # Attributes" << endl;
			for ( auto& a : p.attributes )
			{
				cout << "  " << to_string( a.first ) << ": " << a.second << endl;
			}

			cout << "indices[" << p.indices_index << "] "
			     << "material[" << p.material_index << "] "
			     << "mode[" << to_string( p.mode ) << "] " << endl;
		}
	}
}


void print_scenes( Gltf& model )
{
	cout << endl << "# Scenes" << endl;

	for ( const auto& s : model.scenes )
	{
		cout << s.name << endl;

		cout << "nodes[ ";
		for ( const auto& n : s.nodes )
		{
			cout << n << " ";
		}
		cout << "]" << endl;
	}

	cout << endl << "# Scene[" << model.scene->name << "]" << endl;
}


void printNodes( Gltf& model )
{
	cout << endl << "# Nodes" << endl;

	for ( const auto& node : model.nodes )
	{
		cout << node.name << endl;

		cout << "children[ ";
		for ( const auto& v : node.children )
		{
			cout << v << " ";
		}
		cout << "]" << endl;

		cout << "matrix[ ";
		for ( const auto& v : node.matrix.matrix )
		{
			cout << v << " ";
		}
		cout << "]" << endl;

		cout << "rotation[ " << node.rotation.x << ", " << node.rotation.y << ", " << node.rotation.z << ", "
		     << node.rotation.w << " ]" << endl;

		cout << "scale[ " << node.scale.x << ", " << node.scale.y << ", " << node.scale.z << " ]" << endl;

		cout << "translation[ " << node.translation.x << ", " << node.translation.y << ", " << node.translation.z << " ]"
		     << endl;
	}
}


int main( int argc, char** argv )
{
	/// Check arguments
	if ( argc < 2 )
	{
		cerr << "Usage: " << argv[0] << " modelPath" << endl;
		return EXIT_FAILURE;
	}

	// Json
	cout << "# Json" << endl;
	string modelPath{ argv[1] };
	printJson( modelPath );

	// Gltf
	auto model = Gltf::load( modelPath );
	cout << endl << "# Path[" << model.path << "]" << endl;

	// Asset
	printAsset( model );

	// Buffer
	printBuffer( model );

	// BufferView
	printBufferViews( model );

	// Cameras
	printCameras( model );

	// Samplers
	printSamplers( model );

	// Images
	printImages( model );

	// Textures
	printTextures( model );

	// Accessors
	printAccessors( model );

	// Materials
	printMaterials( model );

	// Meshes
	printMeshes( model );

	// Nodes
	printNodes( model );

	// Schenes
	print_scenes( model );

	return EXIT_SUCCESS;
}
