#include <filespot/Ifstream.h>
#include <fstream>
#include <stdexcept>

#include "gltfspot/Gltf.h"

using namespace std;
using namespace nlohmann;
namespace fst = filespot;

namespace gltfspot
{
Gltf::Gltf( Gltf&& other )
    : asset{ std::move( other.asset ) }
    , mPath{ std::move( other.mPath ) }
    , mBuffers{ std::move( other.mBuffers ) }
    , mBuffersCache{ std::move( other.mBuffersCache ) }
    , mBufferViews{ std::move( other.mBufferViews ) }
    , mCameras{ std::move( other.mCameras ) }
    , mSamplers{ std::move( other.mSamplers ) }
    , mImages{ std::move( other.mImages ) }
    , mTextures{ std::move( other.mTextures ) }
    , mAccessors{ std::move( other.mAccessors ) }
    , mMaterials{ std::move( other.mMaterials ) }
    , mMeshes{ std::move( other.mMeshes ) }
    , lights{ std::move( other.lights ) }
    , mNodes{ std::move( other.mNodes ) }
    , animations{ std::move( other.animations ) }
    , mScenes{ std::move( other.mScenes ) }
    , mScene{ std::move( other.mScene ) }
{
	std::for_each( std::begin( mNodes ), std::end( mNodes ), [this]( auto& node ) { node.gltf = this; } );
	std::for_each( std::begin( mScenes ), std::end( mScenes ), [this]( auto& scene ) { scene.gltf = this; } );
}


Gltf& Gltf::operator=( Gltf&& other )
{
	asset         = std::move( other.asset );
	mPath         = std::move( other.mPath );
	mBuffers      = std::move( other.mBuffers );
	mBuffersCache = std::move( other.mBuffersCache );
	mBufferViews  = std::move( other.mBufferViews );
	mCameras      = std::move( other.mCameras );
	mSamplers     = std::move( other.mSamplers );
	mImages       = std::move( other.mImages );
	mTextures     = std::move( other.mTextures );
	mAccessors    = std::move( other.mAccessors );
	mMaterials    = std::move( other.mMaterials );
	mMeshes       = std::move( other.mMeshes );
	lights        = std::move( other.lights );
	mNodes        = std::move( other.mNodes );
	animations    = std::move( other.animations );
	mScenes       = std::move( other.mScenes );
	mScene        = std::move( other.mScene );

	std::for_each( std::begin( mNodes ), std::end( mNodes ), [this]( auto& node ) { node.gltf = this; } );
	std::for_each( std::begin( mScenes ), std::end( mScenes ), [this]( auto& scene ) { scene.gltf = this; } );

	return *this;
}


Gltf::Gltf( const json& j, const string& path )
{
	// Get the directory path
	auto index = path.find_last_of( "/\\" );
	mPath      = path.substr( 0, index );

	// Asset
	initAsset( j["asset"] );

	// Buffer
	if ( j.count( "buffers" ) )
	{
		initBuffers( j["buffers"] );
	}

	// BufferViews
	if ( j.count( "bufferViews" ) )
	{
		initBufferViews( j["bufferViews"] );
	}

	// Cameras
	if ( j.count( "cameras" ) )
	{
		initCameras( j["cameras"] );
	}

	// Samplers
	if ( j.count( "samplers" ) )
	{
		initSamplers( j["samplers"] );
	}

	// Images
	if ( j.count( "images" ) )
	{
		initImages( j["images"] );
	}

	// Textures
	if ( j.count( "textures" ) )
	{
		initTextures( j["textures"] );
	}

	// Accessors
	initAccessors( j["accessors"] );

	// Materials
	if ( j.count( "materials" ) )
	{
		initMaterials( j["materials"] );
	}

	// Meshes
	if ( j.count( "meshes" ) )
	{
		initMeshes( j["meshes"] );
	}

	// Nodes
	if ( j.count( "nodes" ) )
	{
		initNodes( j["nodes"] );
	}

	// Animations
	if ( j.count( "animations" ) )
	{
		init_animations( j["animations"] );
	}

	// Scenes
	if ( j.count( "scenes" ) )
	{
		initScenes( j["scenes"] );

		uint64_t uIndex = 0;

		if ( j.count( "scene" ) )
		{
			uIndex = j["scene"].get<uint64_t>();
		}
		mScene = &mScenes[static_cast<const unsigned>( uIndex )];
	}

	// Extensions
	if ( j.count( "extensions" ) )
	{
		auto extensions = j["extensions"];

		// Lights
		if ( extensions.count( "KHR_lights_punctual" ) )
		{
			init_lights( extensions["KHR_lights_punctual"]["lights"] );
		}
	}
}


void Gltf::initAsset( const json& j )
{
	// Version (mandatory)
	asset.version = j["version"].get<string>();

	// Generator
	if ( j.count( "generator" ) )
	{
		asset.generator = j["generator"].get<string>();
	}

	// Copyright
	if ( j.count( "copyright" ) )
	{
		asset.copyright = j["copyright"].get<string>();
	}
}


void Gltf::initBuffers( const json& j )
{
	for ( const auto& b : j )
	{
		Buffer buffer;

		// Buffer length in bytes (mandatory)
		buffer.byteLength = b["byteLength"].get<size_t>();

		// Uri of the binary file to upload
		if ( b.count( "uri" ) )
		{
			buffer.uri = b["uri"].get<string>();
			// If it is not data
			if ( buffer.uri.rfind( "data:", 0 ) != 0 )
			{
				buffer.uri = mPath + "/" + buffer.uri;
			}
		}

		mBuffers.push_back( buffer );
	}
}


void Gltf::initBufferViews( const json& j )
{
	for ( const auto& v : j )
	{
		BufferView view;

		// Buffer
		view.buffer = v["buffer"].get<size_t>();

		// Byte offset
		if ( v.count( "byteOffset" ) )
		{
			view.byteOffset = v["byteOffset"].get<size_t>();
		}

		// Byte length
		if ( v.count( "byteLength" ) )
		{
			view.byteLength = v["byteLength"].get<size_t>();
		}

		// Byte stride
		if ( v.count( "byteStride" ) )
		{
			view.byteStride = v["byteStride"].get<size_t>();
		}

		// Target
		if ( v.count( "target" ) )
		{
			view.target = static_cast<Gltf::BufferView::Target>( v["target"].get<size_t>() );
		}

		mBufferViews.push_back( move( view ) );
	}
}


void Gltf::initCameras( const json& j )
{
	for ( const auto& c : j )
	{
		Camera camera;

		// Type
		auto type   = c["type"].get<string>();
		camera.type = ( type == "orthographic" ) ? Camera::Type::Ortographic : Camera::Type::Perspective;

		// Camera
		if ( camera.type == Camera::Type::Ortographic )
		{
			camera.orthographic.xmag  = c["orthographic"]["xmag"].get<float>();
			camera.orthographic.ymag  = c["orthographic"]["ymag"].get<float>();
			camera.orthographic.zfar  = c["orthographic"]["zfar"].get<float>();
			camera.orthographic.znear = c["orthographic"]["znear"].get<float>();
		}
		else
		{
			camera.perspective.aspectRatio = c["perspective"]["aspectRatio"].get<float>();
			camera.perspective.yfov        = c["perspective"]["yfov"].get<float>();
			camera.perspective.zfar        = c["perspective"]["zfar"].get<float>();
			camera.perspective.znear       = c["perspective"]["znear"].get<float>();
		}

		// Name
		if ( c.count( "name" ) )
		{
			camera.name = c["name"].get<string>();
		}

		mCameras.push_back( move( camera ) );
	}
}


template <>
string to_string<Gltf::Sampler::Filter>( const Gltf::Sampler::Filter& f )
{
	switch ( f )
	{
		case Gltf::Sampler::Filter::NONE:
			return "NONE";
		case Gltf::Sampler::Filter::NEAREST:
			return "NEAREST";
		case Gltf::Sampler::Filter::LINEAR:
			return "LINEAR";
		case Gltf::Sampler::Filter::NEAREST_MIPMAP_NEAREST:
			return "NEAREST_MIPMAP_NEAREST";
		case Gltf::Sampler::Filter::LINEAR_MIPMAP_NEAREST:
			return "LINEAR_MIPMAP_NEAREST";
		case Gltf::Sampler::Filter::NEAREST_MIPMAP_LINEAR:
			return "NEAREST_MIPMAP_LINEAR";
		case Gltf::Sampler::Filter::LINEAR_MIPMAP_LINEAR:
			return "LINEAR_MIPMAP_LINEAR";
		default:
			return "UNDEFINED";
	}
}


template <>
string to_string<Gltf::Sampler::Wrapping>( const Gltf::Sampler::Wrapping& w )
{
	switch ( w )
	{
		case Gltf::Sampler::Wrapping::CLAMP_TO_EDGE:
			return "CLAMP_TO_EDGE";
		case Gltf::Sampler::Wrapping::MIRRORED_REPEAT:
			return "MIRRORED_REPEAT";
		case Gltf::Sampler::Wrapping::REPEAT:
			return "REPEAT";
		default:
			return "UNDEFINED";
	}
}


template <>
string to_string<Gltf::Mesh::Primitive::Mode>( const Gltf::Mesh::Primitive::Mode& m )
{
	switch ( m )
	{
		case Gltf::Mesh::Primitive::Mode::POINTS:
			return "Points";
		case Gltf::Mesh::Primitive::Mode::LINES:
			return "Lines";
		case Gltf::Mesh::Primitive::Mode::LINE_LOOP:
			return "LineLoop";
		case Gltf::Mesh::Primitive::Mode::LINE_STRIP:
			return "LineStrip";
		case Gltf::Mesh::Primitive::Mode::TRIANGLES:
			return "Triangles";
		case Gltf::Mesh::Primitive::Mode::TRIANGLE_STRIP:
			return "TriangleStrip";
		case Gltf::Mesh::Primitive::Mode::TRIANGLE_FAN:
			return "TriangleFan";
		default:
			return "Undefined";
	}
}

void Gltf::initSamplers( const json& j )
{
	for ( const auto& s : j )
	{
		Sampler sampler;

		// Mag Filter
		if ( s.count( "magFilter" ) )
		{
			sampler.magFilter = static_cast<Sampler::Filter>( s["magFilter"].get<int>() );
		}

		// Min Filter
		if ( s.count( "minFilter" ) )
		{
			sampler.minFilter = static_cast<Sampler::Filter>( s["minFilter"].get<int>() );
		}

		// WrapS
		if ( s.count( "wrapS" ) )
		{
			sampler.wrapS = static_cast<Sampler::Wrapping>( s["wrapS"].get<int>() );
		}

		// WrapT
		if ( s.count( "wrapT" ) )
		{
			sampler.wrapT = static_cast<Sampler::Wrapping>( s["wrapT"].get<int>() );
		}

		// Name
		if ( s.count( "name" ) )
		{
			sampler.name = s["name"].get<string>();
		}

		mSamplers.push_back( sampler );
	}
}


void Gltf::initImages( const json& j )
{
	for ( const auto& i : j )
	{
		Image image;

		if ( i.count( "uri" ) )
		{
			image.uri = i["uri"].get<string>();
		}

		if ( i.count( "mimeType" ) )
		{
			image.mime_type = i["mimeType"].get<string>();
		}

		if ( i.count( "bufferView" ) )
		{
			image.buffer_view = i["bufferView"].get<uint32_t>();
		}

		if ( i.count( "name" ) )
		{
			image.name = i["name"].get<string>();
		}

		mImages.push_back( std::move( image ) );
	}
}


void Gltf::initTextures( const json& j )
{
	for ( const auto& t : j )
	{
		Texture texture;

		// Sampler
		if ( t.count( "sampler" ) )
		{
			size_t index{ t["sampler"].get<size_t>() };
			texture.sampler = &mSamplers[index];
		}

		// Image
		if ( t.count( "source" ) )
		{
			size_t index{ t["source"].get<size_t>() };
			texture.source = &mImages[index];
		}

		// Name
		if ( t.count( "name" ) )
		{
			texture.name = t["name"].get<string>();
		}

		mTextures.push_back( texture );
	}
}


template <>
Gltf::Accessor::Type from_string<Gltf::Accessor::Type>( const string& s )
{
	if ( s == "SCALAR" )
	{
		return Gltf::Accessor::Type::SCALAR;
	}
	else if ( s == "VEC2" )
	{
		return Gltf::Accessor::Type::VEC2;
	}
	else if ( s == "VEC3" )
	{
		return Gltf::Accessor::Type::VEC3;
	}
	else if ( s == "VEC4" )
	{
		return Gltf::Accessor::Type::VEC4;
	}
	else if ( s == "MAT2" )
	{
		return Gltf::Accessor::Type::MAT2;
	}
	else if ( s == "MAT3" )
	{
		return Gltf::Accessor::Type::MAT3;
	}
	else if ( s == "MAT4" )
	{
		return Gltf::Accessor::Type::MAT4;
	}
	else
	{
		assert( false );
		return Gltf::Accessor::Type::NONE;
	}
}


template <>
std::string to_string<Gltf::Accessor::Type>( const Gltf::Accessor::Type& t )
{
	if ( t == Gltf::Accessor::Type::SCALAR )
	{
		return "SCALAR";
	}
	else if ( t == Gltf::Accessor::Type::VEC2 )
	{
		return "VEC2";
	}
	else if ( t == Gltf::Accessor::Type::VEC3 )
	{
		return "VEC3";
	}
	else if ( t == Gltf::Accessor::Type::VEC4 )
	{
		return "VEC4";
	}
	else if ( t == Gltf::Accessor::Type::MAT2 )
	{
		return "MAT2";
	}
	else if ( t == Gltf::Accessor::Type::MAT3 )
	{
		return "MAT3";
	}
	else if ( t == Gltf::Accessor::Type::MAT4 )
	{
		return "MAT4";
	}
	else
	{
		assert( false );
		return "NONE";
	}
}


void Gltf::initAccessors( const json& j )
{
	for ( const auto& a : j )
	{
		Accessor accessor;

		// Buffer view
		if ( a.count( "bufferView" ) )
		{
			accessor.bufferView = a["bufferView"].get<size_t>();
		}

		// Byte offset
		if ( a.count( "byteOffset" ) )
		{
			accessor.byteOffset = a["byteOffset"].get<size_t>();
		}

		// Component type
		accessor.componentType = a["componentType"].get<Accessor::ComponentType>();

		// Count
		accessor.count = a["count"].get<size_t>();

		// Type
		accessor.type = from_string<Gltf::Accessor::Type>( a["type"].get<string>() );

		// Max
		if ( a.count( "max" ) )
		{
			for ( const auto& value : a["max"] )
			{
				accessor.max.push_back( value.get<float>() );
			}
		}

		// Min
		if ( a.count( "min" ) )
		{
			for ( const auto& value : a["min"] )
			{
				accessor.min.push_back( value.get<float>() );
			}
		}

		mAccessors.push_back( accessor );
	}
}


void Gltf::initMaterials( const json& j )
{
	for ( const auto& m : j )
	{
		Gltf::Material material;

		// Name
		if ( m.count( "name" ) )
		{
			material.name = m["name"].get<string>();
		}

		// PbrMetallicRoughness
		if ( m.count( "pbrMetallicRoughness" ) )
		{
			auto& mr = m["pbrMetallicRoughness"];

			if ( mr.count( "baseColorFactor" ) )
			{
				material.pbrMetallicRoughness.baseColorFactor = mr["baseColorFactor"].get<vector<float>>();
			}

			if ( mr.count( "baseColorTexture" ) )
			{
				size_t index{ mr["baseColorTexture"]["index"].get<size_t>() };
				material.pbrMetallicRoughness.baseColorTexture = &mTextures[index];
			}

			if ( mr.count( "metallicFactor" ) )
			{
				material.pbrMetallicRoughness.metallicFactor = mr["metallicFactor"].get<float>();
			}

			if ( mr.count( "roughnessFactor" ) )
			{
				material.pbrMetallicRoughness.roughnessFactor = mr["roughnessFactor"].get<float>();
			}
		}

		mMaterials.push_back( material );
	}
}


template <>
Gltf::Mesh::Primitive::Semantic from_string<Gltf::Mesh::Primitive::Semantic>( const std::string& s )
{
	if ( s == "POSITION" )
	{
		return Gltf::Mesh::Primitive::Semantic::POSITION;
	}
	else if ( s == "NORMAL" )
	{
		return Gltf::Mesh::Primitive::Semantic::NORMAL;
	}
	else if ( s == "TANGENT" )
	{
		return Gltf::Mesh::Primitive::Semantic::TANGENT;
	}
	else if ( s == "TEXCOORD_0" )
	{
		return Gltf::Mesh::Primitive::Semantic::TEXCOORD_0;
	}
	else if ( s == "TEXCOORD_1" )
	{
		return Gltf::Mesh::Primitive::Semantic::TEXCOORD_1;
	}
	else if ( s == "COLOR_0" )
	{
		return Gltf::Mesh::Primitive::Semantic::COLOR_0;
	}
	else if ( s == "JOINTS_0" )
	{
		return Gltf::Mesh::Primitive::Semantic::JOINTS_0;
	}
	else if ( s == "WEIGHTS_0" )
	{
		return Gltf::Mesh::Primitive::Semantic::WEIGHTS_0;
	}
	else
	{
		assert( false );
		return Gltf::Mesh::Primitive::Semantic::NONE;
	}
}


template <>
std::string to_string<Gltf::Mesh::Primitive::Semantic>( const Gltf::Mesh::Primitive::Semantic& s )
{
	if ( s == Gltf::Mesh::Primitive::Semantic::POSITION )
	{
		return "Position";
	}
	else if ( s == Gltf::Mesh::Primitive::Semantic::NORMAL )
	{
		return "Normal";
	}
	else if ( s == Gltf::Mesh::Primitive::Semantic::TANGENT )
	{
		return "Tangent";
	}
	else if ( s == Gltf::Mesh::Primitive::Semantic::TEXCOORD_0 )
	{
		return "Texcoord0";
	}
	else if ( s == Gltf::Mesh::Primitive::Semantic::TEXCOORD_1 )
	{
		return "Texcoord1";
	}
	else if ( s == Gltf::Mesh::Primitive::Semantic::COLOR_0 )
	{
		return "Color0";
	}
	else if ( s == Gltf::Mesh::Primitive::Semantic::JOINTS_0 )
	{
		return "Joints0";
	}
	else if ( s == Gltf::Mesh::Primitive::Semantic::WEIGHTS_0 )
	{
		return "Weights0";
	}
	else
	{
		assert( false );
		return "None";
	}
}


void Gltf::initMeshes( const json& j )
{
	for ( const auto& m : j )
	{
		Gltf::Mesh mesh;

		// Name
		if ( m.count( "name" ) )
		{
			mesh.name = m["name"].get<string>();
		}

		// Primitives
		for ( const auto& p : m["primitives"] )
		{
			Gltf::Mesh::Primitive primitive;

			auto attributes = p["attributes"].get<map<string, unsigned>>();

			for ( const auto& a : attributes )
			{
				auto semantic = from_string<Gltf::Mesh::Primitive::Semantic>( a.first );
				primitive.attributes.emplace( semantic, a.second );
			}

			if ( p.count( "indices" ) )
			{
				primitive.indices = p["indices"].get<int32_t>();
			}

			if ( p.count( "material" ) )
			{
				primitive.material = p["material"].get<unsigned>();
			}

			if ( p.count( "mode" ) )
			{
				primitive.mode = p["mode"].get<Gltf::Mesh::Primitive::Mode>();
			}

			mesh.primitives.push_back( primitive );
		}

		mMeshes.push_back( mesh );
	}
}


void Gltf::init_lights( const json& j )
{
	for ( const auto& l : j )
	{
		Gltf::Light light;

		// Name
		if ( l.count( "name" ) )
		{
			light.name = l["name"].get<string>();
		}

		// Color
		if ( l.count( "color" ) )
		{
			auto color = l["color"].get<vector<float>>();
			light.color.Set( color[0], color[1], color[2] );
		}

		// Intensity
		if ( l.count( "intensity" ) )
		{
			light.intensity = l["intensity"].get<float>();
		}

		// Range
		if ( l.count( "range" ) )
		{
			light.range = l["range"].get<float>();
		}

		// Type
		if ( l.count( "type" ) )
		{
			auto type = l["type"].get<string>();
			if ( type == "point" )
			{
				light.type = Light::Type::Point;
			}
			else if ( type == "directional" )
			{
				light.type = Light::Type::Directional;
			}
			else if ( type == "spot" )
			{
				light.type = Light::Type::Spot;

				if ( l.count( "spot" ) )
				{
					const auto& spot = l["spot"];
					if ( spot.count( "innerConeAngle" ) )
					{
						light.spot.inner_cone_angle = l["spot"]["innerConeAngle"].get<float>();
					}
					if ( spot.count( "innerConeAngle" ) )
					{
						light.spot.outer_cone_angle = l["spot"]["outerConeAngle"].get<float>();
					}
				}
			}
			else
			{
				assert( false && "Invalid light type" );
			}
		}

		lights.push_back( light );
	}
}


void Gltf::initNodes( const json& j )
{
	size_t i = 0;

	for ( const auto& n : j )
	{
		Gltf::Node node;
		node.gltf = this;

		// Index
		node.index = i++;

		// Name
		if ( n.count( "name" ) )
		{
			node.name = n["name"].get<string>();
		}

		// Camera
		if ( n.count( "camera" ) )
		{
			unsigned m   = n["camera"];
			node.pCamera = &( mCameras[m] );
		}

		// Children
		if ( n.count( "children" ) )
		{
			node.children_indices = n["children"].get<vector<size_t>>();
		}

		// Matrix
		if ( n.count( "matrix" ) )
		{
			// TODO Improve this
			auto             mvec = n["matrix"].get<vector<float>>();
			array<float, 16> marr;
			for ( unsigned i{ 0 }; i < 16; ++i )
			{
				marr[i] = mvec[i];
			}
			node.matrix = mathspot::Mat4{ marr.data() };
		}

		// Mesh
		if ( n.count( "mesh" ) )
		{
			unsigned m = n["mesh"];
			node.pMesh = &( mMeshes[m] );
		}

		// Rotation
		if ( n.count( "rotation" ) )
		{
			auto qvec     = n["rotation"].get<vector<float>>();
			node.rotation = mathspot::Quat{ qvec[0], qvec[1], qvec[2], qvec[3] };
		}

		// Scale
		if ( n.count( "scale" ) )
		{
			auto s     = n["scale"].get<vector<float>>();
			node.scale = mathspot::Vec3{ s[0], s[1], s[2] };
		}

		// Translation
		if ( n.count( "translation" ) )
		{
			auto t           = n["translation"].get<vector<float>>();
			node.translation = mathspot::Vec3{ t[0], t[1], t[2] };
		}

		// Estensions
		if ( n.count( "extensions" ) )
		{
			auto& extensions = n["extensions"];
			// Lights
			if ( extensions.count( "KHR_lights_punctual" ) )
			{
				node.light_index = extensions["KHR_lights_punctual"]["light"].get<int32_t>();
			}
		}

		mNodes.push_back( node );
	}
}


template <>
gltfspot::Gltf::Animation::Sampler::Interpolation from_string<gltfspot::Gltf::Animation::Sampler::Interpolation>(
    const std::string& i )
{
	if ( i == "LINEAR" )
	{
		return Gltf::Animation::Sampler::Interpolation::Linear;
	}
	else if ( i == "STEP" )
	{
		return Gltf::Animation::Sampler::Interpolation::Step;
	}
	else if ( i == "CUBICSPLINE" )
	{
		return Gltf::Animation::Sampler::Interpolation::Cubicspline;
	}
	assert( false );
	return Gltf::Animation::Sampler::Interpolation::Linear;
}

template <>
gltfspot::Gltf::Animation::Target::Path from_string<gltfspot::Gltf::Animation::Target::Path>( const std::string& p )
{
	if ( p == "translation" )
	{
		return Gltf::Animation::Target::Path::Translation;
	}
	else if ( p == "rotation" )
	{
		return Gltf::Animation::Target::Path::Rotation;
	}
	else if ( p == "scale" )
	{
		return Gltf::Animation::Target::Path::Scale;
	}
	else if ( p == "weights" )
	{
		return Gltf::Animation::Target::Path::Weights;
	}
	assert( false );
	return Gltf::Animation::Target::Path::None;
}

void Gltf::init_animations( const nlohmann::json& j )
{
	for ( auto& a : j )
	{
		Animation animation;

		if ( a.count( "name" ) )
		{
			animation.name = a["name"].get<std::string>();
		}

		for ( auto& s : a["samplers"] )
		{
			Animation::Sampler sampler;

			sampler.input  = s["input"].get<size_t>();
			sampler.output = s["output"].get<size_t>();

			if ( s.count( "interpolation" ) )
			{
				sampler.interpolation =
				    from_string<Animation::Sampler::Interpolation>( s["interpolation"].get<std::string>() );
			}

			animation.samplers.push_back(std::move(sampler));
		}

		for ( auto& c : a["channels"] )
		{
			Animation::Channel channel;

			channel.sampler = c["sampler"].get<size_t>();

			// Target
			auto& t = c["target"];

			if ( t.count( "node" ) )
			{
				channel.target.node_index = t["node"].get<int32_t>();
			}

			channel.target.path = from_string<Animation::Target::Path>( t["path"].get<std::string>() );

			animation.channels.push_back(std::move(channel));
		}

		animations.push_back(std::move(animation));
	}
}

void Gltf::load_nodes()
{
	// Reset parents
	std::for_each( std::begin( mNodes ), std::end( mNodes ), []( auto& node ) { node.parent = nullptr; } );

	for ( auto& node : mNodes )
	{
		// Solve nodes children
		node.children.clear();

		for ( auto child_index : node.children_indices )
		{
			auto child = &mNodes[child_index];
			// A node should not be its own parent
			if ( child != &node )
			{
				child->parent = &node;
				node.children.push_back( child );
			}
		}

		// Solve node light
		if ( node.light_index >= 0 )
		{
			node.light = &lights[node.light_index];
		}
	}

	// Solve animations channels target
	for ( auto& animation : animations )
	{
		for ( auto& channel : animation.channels )
		{
			if ( channel.target.node_index >= 0 )
			{
				auto node           = &mNodes[channel.target.node_index];
				channel.target.node = node;
			}
		}
	}

	// Solve scene nodes
	for ( auto& scene : mScenes )
	{
		scene.nodes.clear();

		for ( auto node_index : scene.nodes_indices )
		{
			auto node = &mNodes[node_index];
			scene.nodes.push_back( node );
		}
	}
}

Gltf::Node Gltf::create_node( const std::string& name )
{
	auto node = Node();
	node.name = name;
	node.gltf = this;

	// Assign next index as vector position
	node.index = mNodes.size();

	return node;
}

Gltf::Node& Gltf::add_node( Node&& node )
{
	// Add it to the vector which triggers need to recalculate node references
	mNodes.emplace_back( std::move( node ) );
	load_nodes();
	return mNodes.back();
}

Gltf::Node& Gltf::Scene::create_node( const std::string& name )
{
	auto node = gltf->create_node( name );
	nodes_indices.push_back( node.index );
	return gltf->add_node( std::move( node ) );
}

Gltf::Node& Gltf::Node::create_child( const std::string& name )
{
	auto node = gltf->create_node( name );
	children_indices.push_back( node.index );
	return gltf->add_node( std::move( node ) );
}


void Gltf::Node::remove_from_parent()
{
	if ( parent )
	{
		// Remove node from parent's children
		auto index_it = std::find( std::begin( parent->children_indices ), std::end( parent->children_indices ), index );
		if ( index_it != std::end( parent->children_indices ) )
		{
			parent->children_indices.erase( index_it );
		}

		auto node_it = std::find( std::begin( parent->children ), std::end( parent->children ), this );
		if ( node_it != std::end( parent->children ) )
		{
			parent->children.erase( node_it );
		}

		parent = nullptr;
	}
	else if ( auto scene = gltf->GetScene() )
	{
		// Remove node from the scene
		auto index_it = std::find( std::begin( scene->nodes_indices ), std::end( scene->nodes_indices ), index );
		if ( index_it != std::end( scene->nodes_indices ) )
		{
			scene->nodes_indices.erase( index_it );
		}

		auto node_it = std::find( std::begin( scene->nodes ), std::end( scene->nodes ), this );
		if ( node_it != std::end( scene->nodes ) )
		{
			scene->nodes.erase( node_it );
		}
	}
}


void Gltf::initScenes( const json& j )
{
	for ( const auto& s : j )
	{
		Gltf::Scene scene;
		scene.gltf = this;

		// Name
		if ( s.count( "name" ) )
		{
			scene.name = s["name"].get<string>();
		}

		// Nodes
		if ( s.count( "nodes" ) )
		{
			scene.nodes_indices = s["nodes"].get<vector<size_t>>();
		}

		mScenes.push_back( scene );
	}

	load_nodes();
}

const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";


inline bool is_base64( const char c )
{
	return ( isalnum( c ) || ( c == '+' ) || ( c == '/' ) );
}

std::vector<char> base64_decode( const std::string& encoded_string )
{
	auto              in_len = encoded_string.size();
	int               i      = 0;
	int               j      = 0;
	int               in_    = 0;
	char              char_array_4[4], char_array_3[3];
	std::vector<char> ret;

	while ( in_len-- && ( encoded_string[in_] != '=' ) && is_base64( encoded_string[in_] ) )
	{
		char_array_4[i++] = encoded_string[in_];
		in_++;
		if ( i == 4 )
		{
			for ( i = 0; i < 4; i++ )
			{
				char_array_4[i] = base64_chars.find( char_array_4[i] );
			}

			char_array_3[0] = ( char_array_4[0] << 2 ) + ( ( char_array_4[1] & 0x30 ) >> 4 );
			char_array_3[1] = ( ( char_array_4[1] & 0xf ) << 4 ) + ( ( char_array_4[2] & 0x3c ) >> 2 );
			char_array_3[2] = ( ( char_array_4[2] & 0x3 ) << 6 ) + char_array_4[3];

			for ( i = 0; ( i < 3 ); i++ )
			{
				ret.push_back( char_array_3[i] );
			}
			i = 0;
		}
	}

	if ( i )
	{
		for ( j = i; j < 4; j++ )
		{
			char_array_4[j] = 0;
		}

		for ( j = 0; j < 4; j++ )
		{
			char_array_4[j] = base64_chars.find( char_array_4[j] );
		}

		char_array_3[0] = ( char_array_4[0] << 2 ) + ( ( char_array_4[1] & 0x30 ) >> 4 );
		char_array_3[1] = ( ( char_array_4[1] & 0xf ) << 4 ) + ( ( char_array_4[2] & 0x3c ) >> 2 );
		char_array_3[2] = ( ( char_array_4[2] & 0x3 ) << 6 ) + char_array_4[3];

		for ( j = 0; ( j < i - 1 ); j++ )
		{
			ret.push_back( char_array_3[j] );
		}
	}

	return ret;
}


auto Gltf::loadBuffer( const size_t i )
{
	auto& b = mBuffers[i];

	// Check if it is data
	if ( b.uri.rfind( "data:", 0 ) == 0 )
	{
		// It is data, find the position of comma
		auto comma_pos = b.uri.find_first_of( ',', 5 );
		if ( comma_pos == std::string::npos )
		{
			// Error, data not good
			throw std::runtime_error{ "Data URI not valid" };
		}

		// Assume it is base64
		auto buffer = base64_decode( b.uri.substr( comma_pos + 1 ) );
		return mBuffersCache.emplace( i, move( buffer ) );
	}

	fst::Ifstream file{ b.uri, ios::binary };
	auto          buffer = file.Read( b.byteLength );

	return mBuffersCache.emplace( i, move( buffer ) );
}


Gltf Gltf::Load( const string& path )
{
	// read a JSON file
	fst::Ifstream i{ path };
	json          j;
	i >> j;
	Gltf model{ j, path };
	return model;
}


vector<char>& Gltf::GetBuffer( const size_t i )
{
	if ( mBuffersCache.count( i ) )
	{
		return mBuffersCache[i];
	}

	auto pair = loadBuffer( i );  // (iterator, bool)
	if ( pair.second )            // success
	{
		// (key, value)
		return ( pair.first->second );
	}

	throw out_of_range{ "Could not find the buffer" };
}


const std::string& Gltf::GetPath()
{
	return mPath;
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


vector<Gltf::Texture>& Gltf::GetTextures()
{
	return mTextures;
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

vector<Gltf::Light>& Gltf::get_lights()
{
	return lights;
}

vector<Gltf::Animation>& Gltf::get_animations()
{
	return animations;
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

}  // namespace gltfspot
