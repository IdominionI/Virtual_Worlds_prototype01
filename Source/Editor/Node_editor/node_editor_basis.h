#pragma once

//#include "../Common/definitions.h"
#include "node_editor_definitions.h"
#include "ImNodes/Kernal/node.h"
#include "ImNodes/Kernal/graph.h"

class editor_node_basis {
public:
	node_type_struct editor_node_definition;

	virtual void create_editor_node(node_type_struct ui_node_type, graph_class *graph, scene_manager_class *scene_manager, log_panel_class *log_panel, ImVec2 click_pos) {}

};

