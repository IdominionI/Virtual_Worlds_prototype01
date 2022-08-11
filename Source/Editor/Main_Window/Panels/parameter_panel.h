#pragma once

#include "../../Common/definitions.h"

#include <Universal/ImGui/imgui.h>
#include "../Widgets/imgui_widgets.h"

#include "../Outliner/outliner_manager.h"

#include "../../Node_editor/ImNodes/Kernal/node.h"

// Include the entity data type main parameter widget class to be displayed in
// the application here.
#include <Source/Modules/Module_Voxel_Byte/Editor/Widgets/parameters_widget.h>
#include <Source/Modules/Module_Hex_Surface/Editor/Widgets/hex_parameters_widget.h>

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

	//id_type          *current_selected_object_id      = NULL;
	//id_type          *current_selected_object_type_id = NULL;

	log_panel_class *log_panel  = NULL; // Define the application log panel to display application messages to
	log_panel_class *code_panel = NULL; // Define the application log panel to display application shder or other code error messages to

	//void show() {
	void show(node_basis_class *node = NULL) {

		//if (current_selected_object_type_id == NULL || current_selected_object_id == NULL) {
		//	if (log_panel != NULL) log_panel->application_log.AddLog("ERROR: Parameter panel :No Entity variable defined \n");
		//	return;
		//}

		ImGui::Begin("Object Parameters");

		title("Object Parameters");

		//switch (*current_selected_object_type_id) {
		switch (globalc::get_current_selected_entity_type_id()) {
			case ENTITY_CATEGORY_HCP_VOXEL : display_voxel_hcp_parameters_widget(node);   break;
			//case ENTITY_CATEGORY_HCP_VOXEL : display_voxel_hcp_parameters_widget();   break;
			case ENTITY_CATEGORY_HEX_SURF  : display_hex_surface_parameters_widget(node); break;
			
				//other types to go here		
		}

		ImGui::End();
	}

private:
	// Classes must be defined here and not within the display functions
	// otherwise cannot perform any of the tasks or generate data etc.
	voxel_hcp_parameters_widget_class   voxel_hcp_parameters_widget; 
	hex_surface_parameters_widget_class hex_surface_parameters_widget;

	void display_voxel_hcp_parameters_widget(node_basis_class *node = NULL) {
	//void display_voxel_hcp_parameters_widget(node_basis_class) {
		voxel_hcp_parameters_widget.scene_manager = scene_manager;
		voxel_hcp_parameters_widget.log_panel     = log_panel;
//printf("parameter_panel_class :: display_voxel_hcp_parameters_widget 0000 %i \n", *current_selected_object_id);
		if (node != NULL)
			node->display_ui(scene_manager, log_panel, globalc::get_current_selected_entity_id());
		else
			voxel_hcp_parameters_widget.display_voxel_hcp_generation_widget(globalc::get_current_selected_entity_id());
	}

	//void display_hex_surface_parameters_widget() {
	void display_hex_surface_parameters_widget(node_basis_class *node = NULL) {
		hex_surface_parameters_widget.scene_manager = scene_manager;
		hex_surface_parameters_widget.log_panel     = log_panel;

		if (node != NULL)
			node->display_ui(scene_manager, log_panel, globalc::get_current_selected_entity_id());
		else
			hex_surface_parameters_widget.display_hex_surface_generation_widget(globalc::get_current_selected_entity_id());
	}
};