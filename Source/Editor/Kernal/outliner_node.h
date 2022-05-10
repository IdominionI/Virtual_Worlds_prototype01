#pragma once

#include "../Common/definitions.h"

#include <vector>

enum class outliner_node_type_enum {root, group, entity };

class outliner_node_class {
public:
	outliner_node_class(outliner_node_class *_parent = NULL) {
		parent             = _parent;
		outliner_node_type = outliner_node_type_enum::entity;
		entity_id          = -1;
		name               = "Node";
		category_id        = -1;
		visible            = true;
		selected           = false;
		description        = "Node";
	}

	~outliner_node_class() {
		if(parent != NULL) delete_parent_child_entry();
	}

	outliner_node_class				 *parent;
	std::vector<outliner_node_class*> children;

	outliner_node_type_enum outliner_node_type;					
	object_id_type			entity_id;
	name_type				name;
	category_id_type		category_id;
	bool				    visible;
	bool					selected;
	description_type		description;

	void delete_parent_child_entry() {
		if (parent != NULL) {
			int i = parent->find_child_index(entity_id);

			if (i > -1) {
				parent->children.erase(parent->children.begin() + i);
				parent->children.shrink_to_fit();
			}
		}
	}

	int find_child_index(outliner_node_class *node) {
		for (int i = 0; i < children.size(); i++) {
			if (children[i] == node)
				return i;
		}

		return -1;
	}

	int find_child_index(object_id_type entity_id_to_find) {
		for (int i = 0; i < children.size(); i++) {
			if (children[i]->entity_id == entity_id_to_find)
				return i;
		}

		return -1;
	}

	outliner_node_class *find_child(object_id_type entity_id_to_find) {
		for (int i = 0; i < children.size(); i++) {
			printf("outliner_node_class :: find_child %i : %i\n", children[i]->entity_id , entity_id_to_find);

			if (children[i]->entity_id == entity_id_to_find)
				return children[i];
		}

		return NULL;
	}

private:


};