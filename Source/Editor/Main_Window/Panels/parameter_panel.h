#pragma once

#include <Universal/ImGui/imgui.h>

#include "../Widgets/imgui_widgets.h"
#include "../Outliner/outliner_manager.h"
#include "../../Node_editor/ImNodes/Kernal/node.h"
#include "../Widgets/parameters_widget.h"

/*
							PARAMETER PANEL

	Parent class to define the application parameter panel that is the parent 
	"window widget" for the application to display the entity parameter data
	into and enable whatever functions. procedures etc top be performed for
	that entity
*/

class parameter_panel_class {
public:
	parameter_panel_class() {

	}

	~parameter_panel_class() {}

	scene_manager_class *scene_manager = NULL;

	parameters_widget_manager_class parameters_widget_manager;//*****

	log_panel_class *log_panel  = NULL; // Define the application log panel to display application messages to
	log_panel_class *code_panel = NULL; // Define the application log panel to display application shder or other code error messages to

	void show(node_basis_class *node = NULL) {
		ImGui::Begin("Object Parameters");

		title("Object Parameters");

		object_basis_class* object = scene_manager->get_entity_object(globalc::get_current_selected_entity_id(), globalc::get_current_selected_entity_type_id());
		if (object != NULL) {
//printf("parameter_panel_class : show : object!=NULL %i : %i\n", globalc::get_current_selected_entity_id(), globalc::get_current_selected_entity_type_id());
			parameters_widget_manager.display_entity_parameters(globalc::get_current_selected_entity_id(), globalc::get_current_selected_entity_type_id());
		}
		else {
//printf("parameter_panel_class : show : object==NULL %i : %i\n", globalc::get_current_selected_entity_id(), globalc::get_current_selected_entity_type_id());
			if (node != NULL)
				node->display_ui(scene_manager, log_panel, globalc::get_current_selected_entity_id());
		}

		ImGui::End();
	}

};