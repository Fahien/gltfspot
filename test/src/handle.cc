#include "test.h"

#include <spot/gltf/node.h>
#include <functional>

#define FOREACH( c, f ) ( std::for_each( std::begin( c ), std::end( c ), f ) )

namespace spot::gfx
{

TEST_CASE( "create-handle" )
{
	Uvec<Node> nodes;
	std::vector<Handle<Node>> node_handles;
	const size_t max_nodes = 64;

	for ( size_t i = 0; i < max_nodes; ++i )
	{
		node_handles.emplace_back( nodes.push() );
	}
	
	auto valid = []( const auto& node ){ REQUIRE( node ); };
	FOREACH( node_handles, valid );
}

} // namespace spot::gfx
