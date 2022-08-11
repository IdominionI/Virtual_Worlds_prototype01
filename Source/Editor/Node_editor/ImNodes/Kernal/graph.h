#pragma once
#include <vector>

#include "node.h"

#include "../../../Kernal/id_key_manager.h"

class graph_class {
public:

	nodes_class nodes;
	pins_class  pins;
	links_class links;

	int  add_node(node_basis_class *node, ImVec2 click_pos, id_type entity_id) {
		if (node == NULL) return INVALID_ID;

		node->node_id = node_id_key.get_available_id_key();
//printf("graph_class::add_node:: 000 %i \n", node->node_id);
		node->graph = this;//*****

		node->define_inputs(pins);
		node->define_outputs(pins);
		node->define_node(click_pos,entity_id);
		nodes.insert(node);

//printf("graph_class::add_node:: 111 %i \n", node->node_id);
		return node->node_id;
	}

	void delete_node(node_basis_class* node) {}

	void delete_node(int node_id) {
		//int node_index = nodes.get_node_index(node_id);
		node_basis_class *node = nodes.get_node(node_id);
		if (node == NULL) {
//printf("graph_class::delete_node:: AAAAA node == NULL %i : %i\n", node_id, nodes.nodes.size());
			return;
		}
//printf("graph_class::delete_node:: 000 %i \n", node->node_id);
		node->delete_node_links(links);
//printf("graph_class::delete_node:: 111 %i \n", node->node_id);
		node->delete_node_pins(pins);
//printf("graph_class::delete_node:: 222 %i \n", node->node_id);
		node->delete_node_entity();

		nodes.delete_node(node_id);
		node_id_key.assign_free_id_key(node_id);
	}

	bool node_exists(int entity_category, int entity_context, int entity_id) {
		for (node_basis_class *node : nodes.nodes) {
			if (node->entity_id             == entity_id &&
				node->ui_node_type.category == entity_category &&
				node->ui_node_type.context  == entity_context) {
				return true;
			}
		}
		return false;
	}

	link_struct_type find_link(int link_id) {
		return links.find_link(link_id);
	}

	link_struct_type find_link_with_start_pin(int pin_id) {
		return links.find_link_with_start_pin(pin_id);
	}

	link_struct_type find_link_with_end_pin(int pin_id) {
		return links.find_link_with_end_pin(pin_id);
	}

	//int  add_link(int from, int to) {}
	//void delete_link(int link_id) {}

protected:

private:
	id_key_manager_class<unsigned int> node_id_key;




};