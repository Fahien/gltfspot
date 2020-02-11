#include "spot/gltf/node.h"

#include "spot/gltf/gltf.h"

namespace spot::gltf
{


Node& Node::create_child( const std::string& name )
{
	auto node = model->create_node( name );
	children_indices.push_back( node.index );
	return model->add_node( std::move( node ) );
}


void Node::remove_from_parent()
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
	else if ( auto scene = model->scene )
	{
		// Remove node from the scene
		auto index_it = std::find( std::begin( scene->nodes ), std::end( scene->nodes ), index );
		if ( index_it != std::end( scene->nodes ) )
		{
			scene->nodes.erase( index_it );
		}
	}
}


}  // namespace spot::gltf
