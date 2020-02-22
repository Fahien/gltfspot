#include "spot/gltf/node.h"

#include "spot/gltf/gltf.h"

namespace spot::gltf
{


Mesh* Node::get_mesh() const
{
	if ( mesh >= 0 )
	{
		assert( mesh < model->meshes.size() && "Cannot get mesh out of bounds" );
		return &model->meshes[mesh];
	}
	return nullptr;
}


std::vector<Node*> Node::get_children() const
{
	std::vector<Node*> ret( children.size() );

	for( size_t i = 0; i < children.size(); ++i )
	{
		ret[i] = &model->nodes[i];
	}

	return ret;
}


Node& Node::create_child( const std::string& name )
{
	auto node = model->create_node( name );
	children.push_back( node.index );
	return model->add_node( std::move( node ) );
}


void Node::remove_from_parent()
{
	if ( parent )
	{
		// Remove node from parent's children
		auto index_it = std::find( std::begin( parent->children ), std::end( parent->children ), index );
		if ( index_it != std::end( parent->children ) )
		{
			parent->children.erase( index_it );
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
