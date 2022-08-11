#pragma once

#include "log_panel.h"

#include "../Widgets/imgui_widgets.h"

#include "../Widgets/scene_node_editor_widget.h"

class node_editor_panel_class {
public:
	scene_manager_class *scene_manager;

	log_panel_class         *log_panel  = NULL; // Define the application log panel to display application messages to
	scene_node_editor_class  scene_node_editor;

	ImNodesContext *nodes_context = NULL;

	node_basis_class *selected_node = NULL;

	void show() {
		auto flags = ImGuiWindowFlags_MenuBar;

		// The node editor window
		ImGui::Begin("Scene Node Editor", NULL, flags);
			scene_node_editor.log_panel                       = log_panel;
			scene_node_editor.scene_manager                   = scene_manager;
			scene_node_editor.nodes_context                   = nodes_context;
			scene_node_editor.display();
			selected_node = scene_node_editor.selected_node;
		ImGui::End();

	}

private:
};