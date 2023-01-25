#pragma once

#include "node_editor_basis.h"

class nodes_of_category_class {
public:
	~nodes_of_category_class() {
		clear_all_editor_nodes();
	}

	id_type editor_node_category_id;

	std::vector<editor_node_basis*> editor_category_nodes;

	bool add_editor_node(editor_node_basis* editor_node, scene_manager_class* scene_manager, log_panel_class* log_panel) {
		int index = find_editor_node_creation_function(editor_node->editor_node_definition);
		if (index > -1) {
			if (log_panel != NULL) { log_panel->application_log.AddLog("Error : Could not add node to node editor : Have entity node all ready defined.\n"); }
			return false;
		}

		editor_category_nodes.push_back(editor_node);
		return true;
	}

	bool delete_editor_node(node_type_struct editor_node_definition, log_panel_class* log_panel) {
		int index = find_editor_node_creation_function(editor_node_definition);
		if (index <  0 ) {
			if (log_panel != NULL) { log_panel->application_log.AddLog("Error : Could not delete node definition : Does not exist\n"); }
			return false;
		}

		editor_category_nodes.erase(editor_category_nodes.begin() + index);
		return true;
	}

	void clear_all_editor_nodes() {
		for (editor_node_basis *editor_category_node : editor_category_nodes) {
			delete editor_category_node;
		}
		editor_category_nodes.clear();
		editor_category_nodes.shrink_to_fit();
	}

	bool perform_editor_node_creation(node_type_struct editor_node_definition, graph_class *graph, scene_manager_class* scene_manager, log_panel_class* log_panel, ImVec2 click_pos) {
		int index = find_editor_node_creation_function(editor_node_definition);
//printf("nodes_of_category_class :perform_editor_node_creation %i : %i \n ",int(editor_category_nodes.size()), index);
		if (index < 0) {
			if (log_panel != NULL) {}//error_message;
			return false;
		}

		editor_category_nodes[index]->create_editor_node(editor_node_definition, graph, scene_manager, log_panel, click_pos);
		return true;
	}

	int find_editor_node_creation_function(node_type_struct editor_node_definition) {
		for (int i = 0; i < editor_category_nodes.size(); i++) {
			if (editor_category_nodes[i]->editor_node_definition.context == editor_node_definition.context &&
				editor_category_nodes[i]->editor_node_definition.function == editor_node_definition.function &&
				editor_category_nodes[i]->editor_node_definition.function_category == editor_node_definition.function_category
				)
				return i;
		}

		return -1;
	}


};

class node_editor_category_manager_class {
public:
	std::vector<nodes_of_category_class*> nodes_of_category;

	int add_editor_node_category(id_type editor_nodes_category_id, log_panel_class* log_panel) {
		int index = find_editor_node_creation_function_category(editor_nodes_category_id);
		if (index > -1) {
			if (log_panel != NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("node_editor_category_manager_class :: Cound not add editor nodes category. Category currently exists.\n");
			}
			return -1;
		}

		nodes_of_category_class* new_editor_category_nodes = new nodes_of_category_class;
		if (new_editor_category_nodes == NULL) {
			if (log_panel != NULL) {
				//error message
			}
			return -1;
		}

		new_editor_category_nodes->editor_node_category_id = editor_nodes_category_id;

		nodes_of_category.push_back(new_editor_category_nodes);

		index = int(nodes_of_category.size() - 1);

		return index;
	}

	bool add_editor_node(editor_node_basis* editor_node, scene_manager_class* scene_manager, log_panel_class* log_panel) {
		int index = find_editor_node_creation_function_category(editor_node->editor_node_definition.category);
		if (index < 0) {
			index = add_editor_node_category(editor_node->editor_node_definition.category, log_panel);
			if (index < 0) {
				if (log_panel != NULL) {
					//error message
				}
				return false;
			}
		}

		return nodes_of_category[index]->add_editor_node(editor_node, scene_manager, log_panel);
	}

	bool delete_editor_node_category(id_type editor_category_type_id, log_panel_class* log_panel) {
		int index = find_editor_node_creation_function_category(editor_category_type_id);
		if (index < 0) {
			if (log_panel != NULL) {
				//error message
			}
			return false;
		}

		nodes_of_category[index]->clear_all_editor_nodes();

		return true;
	}

	void clear_all_editor_node_creation_functions() {
		for (nodes_of_category_class* node_of_category : nodes_of_category) {
			node_of_category->clear_all_editor_nodes();
			delete node_of_category;
		}

		nodes_of_category.clear();
	}

	bool perform_editor_node_creation(node_type_struct editor_node_definition, graph_class *graph, scene_manager_class* scene_manager, log_panel_class* log_panel, ImVec2 click_pos) {
		int index = find_editor_node_creation_function_category(editor_node_definition.category);
//printf("node_editor_category_manager_class :perform_editor_node_creation %i : %i \n ",int( nodes_of_category.size()), index);
		if (index < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Could not create node in node editor : Could not find nodes of node category\n");
			return false;
		}

		return nodes_of_category[index]->perform_editor_node_creation(editor_node_definition, graph, scene_manager, log_panel, click_pos);
	}

	int find_editor_node_creation_function_category(id_type editor_category_type_id) {
		for (int i = 0; i < nodes_of_category.size(); i++) {
			if (nodes_of_category[i]->editor_node_category_id == editor_category_type_id)
				return i;
		}

		return -1;
	}
};
