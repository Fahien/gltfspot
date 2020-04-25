#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <spot/gltf/gltf.h>

using namespace std;
using namespace spot::gfx;
using namespace nlohmann;


void printJson( const string& path )
{
	ifstream i{ path };
	assert( i.is_open() && "Cannot open json file" );
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
	auto buffer = model.buffers.get_handle( 0 );
	for ( size_t i{ 0 }; i < buffer->data.size(); ++i )
	{
		if ( i != 0 && ( i % 32 ) == 0 )
		{
			cout << endl;
		}
		auto& b = buffer->data[i];
		printf( "%02X ", b & 0xff );
	}
	cout << endl;
}


void printBufferViews( Gltf& model )
{
	cout << endl << "# BufferViews" << endl;
	for ( auto& view : *model.buffer_views )
	{
		cout << view.buffer.get_index() << ", " << view.byte_length << ", ";

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
			case GltfCamera::Type::Ortographic:
			{
				cout << "orthographic" << endl;
				break;
			}
			case GltfCamera::Type::Perspective:
			{
				cout << "perspective" << endl;
				break;
			}
			default:
			{
				cerr << "Invalid GltfCamera::Type " << static_cast<uint32_t>( camera.type ) << endl;
				assert( false );
			}
		}
	}
}


void printSamplers( Gltf& model )
{
	cout << endl << "# Samplers" << endl;

	for ( auto& sampler : *model.samplers )
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

	for ( auto& image : *model.images )
	{
		cout << "Uri[" << image.uri << "]" << endl;
	}
}


void printTextures( Gltf& model )
{
	cout << endl << "# Textures" << endl;

	for ( auto& texture : *model.textures )
	{
		cout << "Sampler[..." << ( texture.sampler ? to_string( texture.sampler->minFilter ) : "Undefined" ) << "] "
		     << "Source[..." << texture.source->uri << "]" << endl;
	}
}


void printAccessors( Gltf& model )
{
	cout << endl << "# Accessors" << endl;
	for ( auto& acc : *model.accessors )
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
	for ( auto& material : *model.materials )
	{
		cout << material.name << endl;

		cout << "[ "
			<< material.pbr.color.r << " "
			<< material.pbr.color.g << " "
			<< material.pbr.color.b << " "
			<< material.pbr.color.a << " ]" << endl;

		if ( auto& texture = material.texture_handle )
		{
			cout << "Texture  :" << texture->source->uri << endl;
		}

		cout << "Metallic :" << material.pbr.metallic << endl;
		cout << "Roughness:" << material.pbr.roughness << endl;
	}
}


void printMeshes( Gltf& model )
{
	cout << endl << "# Meshes" << endl;

	for ( auto& mesh : *model.meshes )
	{
		cout << mesh.name << endl;

		for ( auto& p : mesh.primitives )
		{
			cout << "  # Attributes" << endl;
			for ( auto&[semantic, accessor] : p.attributes )
			{
				cout << "  " << to_string( semantic ) << ": " << accessor.get_index() << endl;
			}

			cout << "indices[" << p.indices_handle.get_index() << "] "
			     << "material[" << p.material.get_index() << "] "
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
			cout << n->name << " ";
		}
		cout << "]" << endl;
	}

	cout << endl << "# Scene[" << model.scene->name << "]" << endl;
}


void printNodes( Gltf& model )
{
	cout << endl << "# Nodes" << endl;

	for ( const auto& node : *model.nodes )
	{
		cout << node.name << endl;

		cout << "children[ ";
		for ( const auto& v : node.get_children() )
		{
			cout << v->name << " ";
		}
		cout << "]" << endl;

		cout << "matrix[ ";
		for ( const auto& v : node.get_matrix().matrix )
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


int test_gltf( const std::string& modelPath )
{
	// Json
	cout << "# Json" << endl;
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
