#include <fstream>
#include <stdexcept>
#include <spot/file/ifstream.h>

#include "spot/gltf/gltf.h"

namespace fl = spot::file;

namespace spot::gltf
{


Gltf::Gltf( Gltf&& other )
: asset{ std::move( other.asset ) }
, path{ std::move( other.path ) }
, buffers{ std::move( other.buffers ) }
, buffers_cache{ std::move( other.buffers_cache ) }
, buffer_views{ std::move( other.buffer_views ) }
, cameras{ std::move( other.cameras ) }
, samplers{ std::move( other.samplers ) }
, images{ std::move( other.images ) }
, textures{ std::move( other.textures ) }
, accessors{ std::move( other.accessors ) }
, materials{ std::move( other.materials ) }
, meshes{ std::move( other.meshes ) }
, lights{ std::move( other.lights ) }
, nodes{ std::move( other.nodes ) }
, animations{ std::move( other.animations ) }
, rects{ std::move( other.rects ) }
, boxes{ std::move( other.boxes ) }
, spheres{ std::move( other.spheres ) }
, bounds{ std::move( other.bounds ) }
, scripts{ std::move( other.scripts ) }
, scenes{ std::move( other.scenes ) }
, scene{ std::move( other.scene ) }
{
	std::for_each( std::begin( nodes ), std::end( nodes ), [this]( auto& node ) { node.model = this; } );
	std::for_each( std::begin( scenes ), std::end( scenes ), [this]( auto& scene ) { scene.model = this; } );
	std::for_each( std::begin( accessors ), std::end( accessors ), [this]( auto& acc ) { acc.model = this; } );
	std::for_each( std::begin( materials ), std::end( materials ), [this]( auto& mat ) { mat.model = this; } );
	std::for_each( std::begin( textures ), std::end( textures ), [this]( auto& tex ) { tex.model = this; } );
	load_meshes();
	load_nodes();
}


Gltf& Gltf::operator=( Gltf&& other )
{
	asset         = std::move( other.asset );
	path          = std::move( other.path );
	buffers       = std::move( other.buffers );
	buffers_cache = std::move( other.buffers_cache );
	buffer_views  = std::move( other.buffer_views );
	cameras       = std::move( other.cameras );
	samplers      = std::move( other.samplers );
	images        = std::move( other.images );
	textures      = std::move( other.textures );
	accessors     = std::move( other.accessors );
	materials     = std::move( other.materials );
	meshes        = std::move( other.meshes );
	lights        = std::move( other.lights );
	nodes         = std::move( other.nodes );
	animations    = std::move( other.animations );
	rects         = std::move( other.rects );
	boxes         = std::move( other.boxes );
	spheres       = std::move( other.spheres );
	bounds        = std::move( other.bounds );
	scripts       = std::move( other.scripts );
	scenes        = std::move( other.scenes );
	scene         = std::move( other.scene );

	std::for_each( std::begin( nodes ), std::end( nodes ), [this]( auto& node ) { node.model = this; } );
	std::for_each( std::begin( scenes ), std::end( scenes ), [this]( auto& scene ) { scene.model = this; } );
	std::for_each( std::begin( accessors ), std::end( accessors ), [this]( auto& acc ) { acc.model = this; } );
	std::for_each( std::begin( materials ), std::end( materials ), [this]( auto& mat ) { mat.model = this; } );
	std::for_each( std::begin( textures ), std::end( textures ), [this]( auto& tex ) { tex.model = this; } );
	load_meshes();
	load_nodes();

	return *this;
}


Gltf::Gltf( const nlohmann::json& j, const std::string& pth )
{
	// Get the directory path
	auto index = pth.find_last_of( "/\\" );
	path = pth.substr( 0, index );

	// Asset
	init_asset( j["asset"] );

	// Buffer
	if ( j.count( "buffers" ) )
	{
		init_buffers( j["buffers"] );
	}

	// BufferViews
	if ( j.count( "bufferViews" ) )
	{
		init_buffer_views( j["bufferViews"] );
	}

	// Cameras
	if ( j.count( "cameras" ) )
	{
		init_cameras( j["cameras"] );
	}

	// Samplers
	if ( j.count( "samplers" ) )
	{
		init_samplers( j["samplers"] );
	}

	// Images
	if ( j.count( "images" ) )
	{
		init_images( j["images"] );
	}

	// Textures
	if ( j.count( "textures" ) )
	{
		init_textures( j["textures"] );
	}

	// Accessors
	init_accessors( j["accessors"] );

	// Materials
	if ( j.count( "materials" ) )
	{
		init_materials( j["materials"] );
	}

	// Meshes
	if ( j.count( "meshes" ) )
	{
		init_meshes( j["meshes"] );
	}

	// Extras
	if ( j.count( "extras" ) )
	{
		auto& extras = j["extras"];

		// Scripts
		if ( extras.count( "scripts" ) )
		{
			init_scripts( extras["scripts"] );
		}

		// Shapes
		if ( extras.count( "shapes" ) )
		{
			init_shapes( extras["shapes"] );
		}
	}

	// Nodes
	if ( j.count( "nodes" ) )
	{
		init_nodes( j["nodes"] );
	}

	// Animations
	if ( j.count( "animations" ) )
	{
		init_animations( j["animations"] );
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

	// Scenes
	if ( j.count( "scenes" ) )
	{
		init_scenes( j["scenes"] );

		uint64_t uIndex = 0;

		if ( j.count( "scene" ) )
		{
			uIndex = j["scene"].get<uint64_t>();
		}
		scene = &scenes[static_cast<const unsigned>( uIndex )];
	}
}


void Gltf::init_asset( const nlohmann::json& j )
{
	// Version (mandatory)
	asset.version = j["version"].get<std::string>();

	// Generator
	if ( j.count( "generator" ) )
	{
		asset.generator = j["generator"].get<std::string>();
	}

	// Copyright
	if ( j.count( "copyright" ) )
	{
		asset.copyright = j["copyright"].get<std::string>();
	}
}


void Gltf::init_buffers( const nlohmann::json& j )
{
	for ( const auto& b : j )
	{
		Buffer buffer;

		// Buffer length in bytes (mandatory)
		buffer.byte_length = b["byteLength"].get<size_t>();

		// Uri of the binary file to upload
		if ( b.count( "uri" ) )
		{
			buffer.uri = b["uri"].get<std::string>();
			// If it is not data
			if ( buffer.uri.rfind( "data:", 0 ) != 0 )
			{
				buffer.uri = path + "/" + buffer.uri;
			}
		}

		buffers.push_back( buffer );
	}
}


void Gltf::init_buffer_views( const nlohmann::json& j )
{
	for ( const auto& v : j )
	{
		BufferView view;
		view.model = this;

		// Buffer
		view.buffer_index = v["buffer"].get<size_t>();

		// Byte offset
		if ( v.count( "byteOffset" ) )
		{
			view.byte_offset = v["byteOffset"].get<size_t>();
		}

		// Byte length
		if ( v.count( "byteLength" ) )
		{
			view.byte_length = v["byteLength"].get<size_t>();
		}

		// Byte stride
		if ( v.count( "byteStride" ) )
		{
			view.byte_stride = v["byteStride"].get<size_t>();
		}

		// Target
		if ( v.count( "target" ) )
		{
			view.target = static_cast<BufferView::Target>( v["target"].get<size_t>() );
		}

		buffer_views.push_back( std::move( view ) );
	}
}


void Gltf::init_cameras( const nlohmann::json& j )
{
	for ( const auto& c : j )
	{
		Camera camera;

		// Type
		auto type   = c["type"].get<std::string>();
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
			auto& perspective = c["perspective"];
			if ( perspective.count( "aspectRatio" ) )
			{
				camera.perspective.aspect_ratio = perspective["aspectRatio"].get<float>();
			}
			camera.perspective.yfov  = c["perspective"]["yfov"].get<float>();
			camera.perspective.zfar  = c["perspective"]["zfar"].get<float>();
			camera.perspective.znear = c["perspective"]["znear"].get<float>();
		}

		// Name
		if ( c.count( "name" ) )
		{
			camera.name = c["name"].get<std::string>();
		}

		cameras.push_back( std::move( camera ) );
	}
}


template <>
std::string to_string<Sampler::Filter>( const Sampler::Filter& f )
{
	switch ( f )
	{
		case Sampler::Filter::NONE:
			return "NONE";
		case Sampler::Filter::NEAREST:
			return "NEAREST";
		case Sampler::Filter::LINEAR:
			return "LINEAR";
		case Sampler::Filter::NEAREST_MIPMAP_NEAREST:
			return "NEAREST_MIPMAP_NEAREST";
		case Sampler::Filter::LINEAR_MIPMAP_NEAREST:
			return "LINEAR_MIPMAP_NEAREST";
		case Sampler::Filter::NEAREST_MIPMAP_LINEAR:
			return "NEAREST_MIPMAP_LINEAR";
		case Sampler::Filter::LINEAR_MIPMAP_LINEAR:
			return "LINEAR_MIPMAP_LINEAR";
		default:
			return "UNDEFINED";
	}
}


template <>
std::string to_string<Sampler::Wrapping>( const Sampler::Wrapping& w )
{
	switch ( w )
	{
		case Sampler::Wrapping::CLAMP_TO_EDGE:
			return "CLAMP_TO_EDGE";
		case Sampler::Wrapping::MIRRORED_REPEAT:
			return "MIRRORED_REPEAT";
		case Sampler::Wrapping::REPEAT:
			return "REPEAT";
		default:
			return "UNDEFINED";
	}
}


template <>
std::string to_string<Mesh::Primitive::Mode>( const Mesh::Primitive::Mode& m )
{
	switch ( m )
	{
		case Mesh::Primitive::Mode::POINTS:
			return "Points";
		case Mesh::Primitive::Mode::LINES:
			return "Lines";
		case Mesh::Primitive::Mode::LINE_LOOP:
			return "LineLoop";
		case Mesh::Primitive::Mode::LINE_STRIP:
			return "LineStrip";
		case Mesh::Primitive::Mode::TRIANGLES:
			return "Triangles";
		case Mesh::Primitive::Mode::TRIANGLE_STRIP:
			return "TriangleStrip";
		case Mesh::Primitive::Mode::TRIANGLE_FAN:
			return "TriangleFan";
		default:
			return "Undefined";
	}
}

void Gltf::init_samplers( const nlohmann::json& j )
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
			sampler.name = s["name"].get<std::string>();
		}

		samplers.push_back( sampler );
	}
}


void Gltf::init_images( const nlohmann::json& j )
{
	for ( const auto& i : j )
	{
		Image image;

		if ( i.count( "uri" ) )
		{
			image.uri = path + "/" + i["uri"].get<std::string>();
		}

		if ( i.count( "mimeType" ) )
		{
			image.mime_type = i["mimeType"].get<std::string>();
		}

		if ( i.count( "bufferView" ) )
		{
			image.buffer_view = i["bufferView"].get<uint32_t>();
		}

		if ( i.count( "name" ) )
		{
			image.name = i["name"].get<std::string>();
		}

		images.push_back( std::move( image ) );
	}
}


void Gltf::init_textures( const nlohmann::json& j )
{
	for ( const auto& t : j )
	{
		Texture texture;
		texture.model = this;

		// Sampler
		if ( t.count( "sampler" ) )
		{
			size_t index{ t["sampler"].get<size_t>() };
			texture.sampler = &samplers[index];
		}

		// Image
		if ( t.count( "source" ) )
		{
			auto index = t["source"].get<int32_t>();
			texture.source_index = index;
		}

		// Name
		if ( t.count( "name" ) )
		{
			texture.name = t["name"].get<std::string>();
		}

		textures.push_back( texture );
	}
}


template <>
Accessor::Type from_string<Accessor::Type>( const std::string& s )
{
	if ( s == "SCALAR" )
	{
		return Accessor::Type::SCALAR;
	}
	else if ( s == "VEC2" )
	{
		return Accessor::Type::VEC2;
	}
	else if ( s == "VEC3" )
	{
		return Accessor::Type::VEC3;
	}
	else if ( s == "VEC4" )
	{
		return Accessor::Type::VEC4;
	}
	else if ( s == "MAT2" )
	{
		return Accessor::Type::MAT2;
	}
	else if ( s == "MAT3" )
	{
		return Accessor::Type::MAT3;
	}
	else if ( s == "MAT4" )
	{
		return Accessor::Type::MAT4;
	}
	else
	{
		assert( false );
		return Accessor::Type::NONE;
	}
}


template <>
std::string to_string<Accessor::Type>( const Accessor::Type& t )
{
	if ( t == Accessor::Type::SCALAR )
	{
		return "SCALAR";
	}
	else if ( t == Accessor::Type::VEC2 )
	{
		return "VEC2";
	}
	else if ( t == Accessor::Type::VEC3 )
	{
		return "VEC3";
	}
	else if ( t == Accessor::Type::VEC4 )
	{
		return "VEC4";
	}
	else if ( t == Accessor::Type::MAT2 )
	{
		return "MAT2";
	}
	else if ( t == Accessor::Type::MAT3 )
	{
		return "MAT3";
	}
	else if ( t == Accessor::Type::MAT4 )
	{
		return "MAT4";
	}
	else
	{
		assert( false );
		return "NONE";
	}
}


Accessor::Accessor( Gltf& g )
: model { &g }
{}


Accessor::Accessor( Accessor&& other )
: model { other.model }
, buffer_view_index { other.buffer_view_index }
, byte_offset { other.byte_offset }
, component_type { other.component_type }
, count { other.count }
, type { other.type }
, max { std::move( other.max ) }
, min { std::move( other.min ) }
{
	other.model = nullptr;
}


BufferView& Accessor::get_buffer_view() const
{
	return model->buffer_views[buffer_view_index];
}


size_t size_of( Accessor::ComponentType ct )
{
	switch ( ct )
	{
	case Accessor::ComponentType::BYTE:          return sizeof( uint8_t );
	case Accessor::ComponentType::UNSIGNED_BYTE: return sizeof( uint8_t );
	case Accessor::ComponentType::SHORT:         return sizeof( uint16_t );
	case Accessor::ComponentType::UNSIGNED_SHORT:return sizeof( uint16_t );
	case Accessor::ComponentType::UNSIGNED_INT:  return sizeof( uint32_t );
	case Accessor::ComponentType::FLOAT:         return sizeof( float );
	default: assert( false && "Invalid accessor component type" ); return 0;
	}
}


size_t size_of( Accessor::Type tp )
{
	switch ( tp )
	{
	case Accessor::Type::NONE: return 1;
	case Accessor::Type::SCALAR: return 1;
	case Accessor::Type::VEC2: return 2;
	case Accessor::Type::VEC3: return 3;
	case Accessor::Type::VEC4: return 4;
	case Accessor::Type::MAT2: return 4;
	case Accessor::Type::MAT3: return 9;
	case Accessor::Type::MAT4: return 16;
	default: assert( false && "Invalid accessor type" ); return 0;
	}
}

size_t Accessor::get_size() const
{
	return count * size_of( component_type ) * size_of( type );
}


const uint8_t* Accessor::get_data() const
{
	auto& buffer_view = get_buffer_view();
	auto& buffer = model->get_buffer( buffer_view.buffer_index );
	auto data = buffer.data.data() + buffer_view.byte_offset + byte_offset;
	return reinterpret_cast<const uint8_t*>( data );
}


size_t Accessor::get_stride() const
{
	auto& buffer_view = model->buffer_views.at( buffer_view_index );
	return buffer_view.byte_stride;
}


void Gltf::init_accessors( const nlohmann::json& j )
{
	for ( const auto& a : j )
	{
		Accessor accessor { *this };

		// Buffer view
		if ( a.count( "bufferView" ) )
		{
			accessor.buffer_view_index = a["bufferView"].get<size_t>();
		}

		// Byte offset
		if ( a.count( "byteOffset" ) )
		{
			accessor.byte_offset = a["byteOffset"].get<size_t>();
		}

		// Component type
		accessor.component_type = a["componentType"].get<Accessor::ComponentType>();

		// Count
		accessor.count = a["count"].get<size_t>();

		// Type
		accessor.type = from_string<Accessor::Type>( a["type"].get<std::string>() );

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

		accessors.push_back( std::move( accessor ) );
	}
}


void Gltf::init_materials( const nlohmann::json& j )
{
	for ( const auto& m : j )
	{
		Material material;
		material.model = this;

		// Name
		if ( m.count( "name" ) )
		{
			material.name = m["name"].get<std::string>();
		}

		// PbrMetallicRoughness
		if ( m.count( "pbrMetallicRoughness" ) )
		{
			auto& mr = m["pbrMetallicRoughness"];

			if ( mr.count( "baseColorFactor" ) )
			{
				material.pbr.base_color_factor = mr["baseColorFactor"].get<std::vector<float>>();
			}

			if ( mr.count( "baseColorTexture" ) )
			{
				int32_t index = mr["baseColorTexture"]["index"].get<int32_t>();
				material.pbr.texture_index = index;
			}

			if ( mr.count( "metallicFactor" ) )
			{
				material.pbr.metallic_factor = mr["metallicFactor"].get<float>();
			}

			if ( mr.count( "roughnessFactor" ) )
			{
				material.pbr.roughness_factor = mr["roughnessFactor"].get<float>();
			}
		}

		materials.push_back( material );
	}
}


template <>
Mesh::Primitive::Semantic from_string<Mesh::Primitive::Semantic>( const std::string& s )
{
	if ( s == "POSITION" )
	{
		return Mesh::Primitive::Semantic::POSITION;
	}
	else if ( s == "NORMAL" )
	{
		return Mesh::Primitive::Semantic::NORMAL;
	}
	else if ( s == "TANGENT" )
	{
		return Mesh::Primitive::Semantic::TANGENT;
	}
	else if ( s == "TEXCOORD_0" )
	{
		return Mesh::Primitive::Semantic::TEXCOORD_0;
	}
	else if ( s == "TEXCOORD_1" )
	{
		return Mesh::Primitive::Semantic::TEXCOORD_1;
	}
	else if ( s == "COLOR_0" )
	{
		return Mesh::Primitive::Semantic::COLOR_0;
	}
	else if ( s == "JOINTS_0" )
	{
		return Mesh::Primitive::Semantic::JOINTS_0;
	}
	else if ( s == "WEIGHTS_0" )
	{
		return Mesh::Primitive::Semantic::WEIGHTS_0;
	}
	else
	{
		assert( false );
		return Mesh::Primitive::Semantic::NONE;
	}
}


template <>
std::string to_string<Mesh::Primitive::Semantic>( const Mesh::Primitive::Semantic& s )
{
	if ( s == Mesh::Primitive::Semantic::POSITION )
	{
		return "Position";
	}
	else if ( s == Mesh::Primitive::Semantic::NORMAL )
	{
		return "Normal";
	}
	else if ( s == Mesh::Primitive::Semantic::TANGENT )
	{
		return "Tangent";
	}
	else if ( s == Mesh::Primitive::Semantic::TEXCOORD_0 )
	{
		return "Texcoord0";
	}
	else if ( s == Mesh::Primitive::Semantic::TEXCOORD_1 )
	{
		return "Texcoord1";
	}
	else if ( s == Mesh::Primitive::Semantic::COLOR_0 )
	{
		return "Color0";
	}
	else if ( s == Mesh::Primitive::Semantic::JOINTS_0 )
	{
		return "Joints0";
	}
	else if ( s == Mesh::Primitive::Semantic::WEIGHTS_0 )
	{
		return "Weights0";
	}
	else
	{
		assert( false );
		return "None";
	}
}


void Gltf::init_meshes( const nlohmann::json& j )
{
	for ( const auto& m : j )
	{
		Mesh mesh { *this };

		// Name
		if ( m.count( "name" ) )
		{
			mesh.name = m["name"].get<std::string>();
		}

		// Primitives
		for ( const auto& p : m["primitives"] )
		{
			Mesh::Primitive primitive { mesh };

			auto attributes = p["attributes"].get<std::map<std::string, unsigned>>();

			for ( const auto& a : attributes )
			{
				auto semantic = from_string<Mesh::Primitive::Semantic>( a.first );
				primitive.attributes.emplace( semantic, a.second );
			}

			if ( p.count( "indices" ) )
			{
				primitive.indices_index = p["indices"].get<int32_t>();
			}

			if ( p.count( "material" ) )
			{
				primitive.material = p["material"].get<int32_t>();
			}

			if ( p.count( "mode" ) )
			{
				primitive.mode = p["mode"].get<Mesh::Primitive::Mode>();
			}

			mesh.primitives.push_back( std::move( primitive ) );
		}

		meshes.push_back( std::move( mesh ) );
	}
}


void Gltf::load_meshes()
{
	for ( auto& mesh : meshes )
	{
		mesh.model = this;
	}
}


void Gltf::init_lights( const nlohmann::json& j )
{
	for ( const auto& l : j )
	{
		Light light;

		// Name
		if ( l.count( "name" ) )
		{
			light.name = l["name"].get<std::string>();
		}

		// Color
		if ( l.count( "color" ) )
		{
			auto color = l["color"].get<std::vector<float>>();
			light.color.set( color[0], color[1], color[2] );
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
			auto type = l["type"].get<std::string>();
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


void Gltf::init_nodes( const nlohmann::json& j )
{
	size_t i = 0;

	for ( const auto& n : j )
	{
		Node node;
		node.model = this;

		// Index
		node.index = i++;

		// Name
		if ( n.count( "name" ) )
		{
			node.name = n["name"].get<std::string>();
		}

		// Camera
		if ( n.count( "camera" ) )
		{
			unsigned m  = n["camera"];
			node.camera = &( cameras[m] );
		}

		// Children
		if ( n.count( "children" ) )
		{
			node.children = n["children"].get<std::vector<int32_t>>();
		}

		// Matrix
		if ( n.count( "matrix" ) )
		{
			/// @todo Improve this
			auto             mvec = n["matrix"].get<std::vector<float>>();
			std::array<float, 16> marr;
			for ( unsigned i{ 0 }; i < 16; ++i )
			{
				marr[i] = mvec[i];
			}
			node.matrix = math::Mat4{ marr.data() };
		}

		// Mesh
		if ( n.count( "mesh" ) )
		{
			node.mesh = n["mesh"];
		}

		// Rotation
		if ( n.count( "rotation" ) )
		{
			auto qvec     = n["rotation"].get<std::vector<float>>();
			node.rotation = math::Quat{ qvec[3], qvec[0], qvec[1], qvec[2] };
		}

		// Scale
		if ( n.count( "scale" ) )
		{
			auto s     = n["scale"].get<std::vector<float>>();
			node.scale = math::Vec3{ s[0], s[1], s[2] };
		}

		// Translation
		if ( n.count( "translation" ) )
		{
			auto t           = n["translation"].get<std::vector<float>>();
			node.translation = math::Vec3{ t[0], t[1], t[2] };
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

		// Extras
		if ( n.count( "extras" ) )
		{
			auto& extras = n["extras"];

			// Bounds
			if ( extras.count( "bounds" ) )
			{
				node.bounds = extras["bounds"].get<int32_t>();
			}

			// Scripts
			if ( extras.count( "scripts" ) )
			{
				node.scripts_indices = extras["scripts"].get<std::vector<size_t>>();
			}
		}

		nodes.push_back( std::move( node ) );
	}
}


template <>
spot::gltf::Gltf::Animation::Sampler::Interpolation from_string<spot::gltf::Gltf::Animation::Sampler::Interpolation>(
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
spot::gltf::Gltf::Animation::Target::Path from_string<spot::gltf::Gltf::Animation::Target::Path>( const std::string& p )
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

			animation.samplers.push_back( std::move( sampler ) );
		}

		for ( auto& c : a["channels"] )
		{
			Animation::Channel channel;

			channel.sampler = c["sampler"].get<size_t>();

			// Target
			auto& t = c["target"];

			if ( t.count( "node" ) )
			{
				channel.target.node = t["node"].get<int32_t>();
			}

			channel.target.path = from_string<Animation::Target::Path>( t["path"].get<std::string>() );

			animation.channels.push_back( std::move( channel ) );
		}

		animations.push_back( std::move( animation ) );
	}
}


template <>
spot::gltf::Bounds::Type from_string<spot::gltf::Bounds::Type>( const std::string& b )
{
	if ( b == "box" )
	{
		return gltf::Bounds::Type::Box;
	}
	else if ( b == "sphere" )
	{
		return gltf::Bounds::Type::Sphere;
	}
	else
	{
		throw std::runtime_error{ "Bounds not valid: " + b };
	}
}


void Gltf::init_shapes( const nlohmann::json& ss )
{
	for ( auto& s : ss )
	{
		auto type = s["type"].get<std::string>();
		if ( type == "box" )
		{
			auto aa = s["box"]["a"].get<std::vector<float>>();
			auto a  = math::Vec3{ aa[0], aa[1], aa[2] };
			auto bb = s["box"]["b"].get<std::vector<float>>();
			auto b  = math::Vec3{ bb[0], bb[1], bb[2] };

			boxes.emplace_back( Box{ a, b } );
		}
		else if ( type == "sphere" )
		{
			auto oo = s["sphere"]["o"].get<std::vector<float>>();
			auto o  = math::Vec3{ oo[0], oo[1], oo[2] };

			auto r = s["sphere"]["r"].get<float>();

			spheres.emplace_back( Sphere{ o, r } );
		}
		else
		{
			throw std::runtime_error{ "Type not supported: " + type };
		}
	}
}


void Gltf::init_scripts( const nlohmann::json& ss )
{
	// Init scripts
	Script script;

	for ( auto& s : ss )
	{
		script.uri = s["uri"].get<std::string>();

		if ( s.count( "name" ) )
		{
			script.name = s["name"].get<std::string>();
		}
		else
		{
			script.name = script.uri;
		}

		scripts.push_back( script );
	}
}


void Gltf::load_nodes()
{
	for ( auto& node : nodes )
	{
		// Solve parents
		for ( auto child_index : node.children )
		{
			nodes[child_index].parent = node.index;
		}

		// Solve node light
		if ( node.light_index >= 0 )
		{
			node.light = &lights[node.light_index];
		}

		// Solve node script
		node.scripts.clear();

		if ( !node.scripts_indices.empty() )
		{
			for ( auto script_index : node.scripts_indices )
			{
				auto script = &scripts[script_index];
				node.scripts.push_back( script );
			}
		}
	}
}


Node& Gltf::create_node( const int32_t parent_index )
{
	auto& node = nodes.emplace_back();
	node.index = nodes.size() - 1;
	node.model = this;
	node.parent = parent_index;

	if ( auto parent = get_node( parent_index ) )
	{
		parent->children.emplace_back( node.index );
	}
	return node;
}


Node& Gltf::create_node( const std::string& name )
{
	auto& node = create_node();
	node.name = name;
	return node;
}


Node& Gltf::add_node( Node&& node )
{
	// Add it to the vector which triggers need to recalculate node references
	nodes.emplace_back( std::move( node ) );
	load_nodes();
	return nodes.back();
}


Node& Scene::create_node( const std::string& name )
{
	auto& node = model->create_node( name );
	nodes.push_back( node.index );
	return node;
}


Node* Gltf::get_node( const int32_t index )
{
	if ( index >= 0 && index < nodes.size() )
	{
		return &nodes[index];
	}
	return nullptr;
}


Shape* Gltf::get_bounds( int32_t index )
{
	if ( index >= 0 && index < bounds.size() )
	{
		auto& bb = bounds[index];
		switch ( bb.type )
		{
		case Bounds::Type::Rect:
			if ( bb.shape >= 0 && bb.shape < rects.size() )
			{
				return &rects[bb.shape];
			}
		default:
			assert( false && "Bounds type not supported yet" );
		}
	}
	return nullptr;
}


void Gltf::init_scenes( const nlohmann::json& j )
{
	for ( const auto& s : j )
	{
		Scene scene;
		scene.model = this;

		// Name
		if ( s.count( "name" ) )
		{
			scene.name = s["name"].get<std::string>();
		}

		// Nodes
		if ( s.count( "nodes" ) )
		{
			scene.nodes = s["nodes"].get<std::vector<int32_t>>();
		}

		scenes.push_back( scene );
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


Buffer& Gltf::load_buffer( const size_t i )
{
	auto& buffer = buffers[i];

	if ( buffer.data.empty() )
	{
		// Check if it is data
		if ( buffer.uri.rfind( "data:", 0 ) == 0 )
		{
			// It is data, find the position of comma
			auto comma_pos = buffer.uri.find_first_of( ',', 5 );
			if ( comma_pos == std::string::npos )
			{
				// Error, data not good
				throw std::runtime_error{ "Data URI not valid" };
			}

			// Assume it is base64
			buffer.data = base64_decode( buffer.uri.substr( comma_pos + 1 ) );
		}
		else
		{
			auto file = fl::Ifstream( buffer.uri, std::ios::binary );
			assert( file.is_open() && "Could not open the file" );
			buffer.data = file.read( buffer.byte_length );
		}
	}

	return buffer;
}


Gltf Gltf::load( const std::string& path )
{
	// read a JSON file
	auto in = fl::Ifstream( path );
	assert( in.is_open() && "Cannot open gltf file" );
	nlohmann::json js;
	in >> js;
	return Gltf( js, path );
}


Buffer& Gltf::get_buffer( const size_t i )
{
	return load_buffer( i );
}


}  // namespace spot::gltf
