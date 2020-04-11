#include "spot/gltf/node.h"

#include "spot/gltf/gltf.h"

namespace spot::gltf
{


bool Node::contains( const math::Vec2& point ) const
{
	/// @todo Make this a bounding box and store an handle to it into a node
	auto rect = math::Rectangle();
	rect.width = 1.0f;
	rect.height = 1.0f;
	rect.x = -rect.width / 2.0f;
	rect.y = -rect.height / 2.0f;

	rect.x += translation.x;
	rect.y += translation.y;

	return rect.contains( point.x, point.y );
}


Node* Node::get_parent() const
{
	return model->get_node( parent );
}


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
	auto& node = model->create_node( name );
	node.parent = index;
	children.push_back( node.index );
	return model->add_node( std::move( node ) );
}


void Node::add_child( Node& child )
{
	child.parent = index;
	children.emplace_back( child.index );
}


void Node::remove_from_parent()
{
	if ( auto parent_node = get_parent() )
	{
		// Remove node from parent's children
		auto index_it = std::find( std::begin( parent_node->children ), std::end( parent_node->children ), index );
		if ( index_it != std::end( parent_node->children ) )
		{
			parent_node->children.erase( index_it );
		}

		parent = -1;
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
