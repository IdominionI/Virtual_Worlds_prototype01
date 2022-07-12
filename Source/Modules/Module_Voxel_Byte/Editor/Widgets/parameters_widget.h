#pragma once

#include <Source/Editor/Scene/scene_manager.h>

//#include "cart_to_voxel_generator.h"
//#include "generator_widget.h"

#include "generation_selection_widget.h"
#include "shaders_widget.h"
#include "automata_widget.h"
//#include "interactions_widget.h"

/*
				HCP voxel volume paramters widget

	This widget class is a parent widget that manages the display
	of HCP voxel generation, shader variable and cellula automata child
	widgets via selection of ImGui tabs.

	This widget class is thus the widget class that is called by
	the main application editor to display hex surface parameter
	data to be changed or generate data.
*/

class voxel_hcp_parameters_widget_class {
public:
	voxel_hcp_parameters_widget_class() {}
	~voxel_hcp_parameters_widget_class() {}

	scene_manager_class *scene_manager = NULL;

	log_panel_class     *log_panel     = NULL;

	void display_voxel_hcp_generation_widget(id_type current_selected_object_id) {
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget 11\n");

		if (scene_manager == NULL) {
		//	if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No scene manager defined to extract scene voxel hcp object parameter data from.\n");
			return;
		}

		if (current_selected_object_id < 0) {
		//	if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Invalid selected ID number given for the current selected hcp voxel object. No object selected\n");
			return;
		}

		voxel_hcp_object_class *voxel_hcp_object_to_execute = scene_manager->entities_manager.get_voxel_hcp_entity_object(current_selected_object_id);	// Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
		
		if (voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform tasks on.\n");
			return;
		}

		if (ImGui::BeginTabBar("##hcp_voxel_tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Generation##hcp_voxel_tab")){
				display_voxel_generation_widget(voxel_hcp_object_to_execute, current_selected_object_id);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Shader##voxel_shader_tab")){
				display_voxel_shader_widget(voxel_hcp_object_to_execute, current_selected_object_id);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Automata##voxel_auto_tab")) {
				display_voxel_automata_widget(voxel_hcp_object_to_execute, current_selected_object_id);
				ImGui::EndTabItem();
			}
/*
			if (ImGui::BeginTabItem("Interactions##voxel_inter_tab")) {
				display_voxel_interactions_widget(voxel_hcp_object_to_execute, current_selected_object_id);
				ImGui::EndTabItem();
			}
*/
			ImGui::EndTabBar();
		}
	}



private:
	//voxel_hcp_generation_widget_class  voxel_hcp_generation_widget;
	voxel_shaders_widget_class		   voxel_shaders_widget;
	voxel_hcp_autmoata_widget_class	   voxel_hcp_autmoata_widget;
//	voxel_hcp_interaction_widget_class voxel_hcp_interaction_widget;

	hcp_voxel_genertion_selection_widget_class hcp_voxel_genertion_selection_widget;

	void display_voxel_generation_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id) {
		hcp_voxel_genertion_selection_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;

		if (hcp_voxel_genertion_selection_widget.voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform voxel generation procedure on.\n");
			return;
		}
//printf("voxel_hcp_generation_widget.voxel_hcp_object_to_execute != NULL ^^^^\n");
		hcp_voxel_genertion_selection_widget.log_panel = log_panel;
		hcp_voxel_genertion_selection_widget.scene_manager = scene_manager;
		hcp_voxel_genertion_selection_widget.current_selected_entity_id = current_selected_object_id;
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget22\n");
		hcp_voxel_genertion_selection_widget.display();
	
	}

	void display_voxel_shader_widget(voxel_hcp_object_class *voxel_hcp_object_to_execute, id_type current_selected_object_id) {
		voxel_shaders_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;
		//voxel_shaders_widget.voxel_hcp_generation_widget = &voxel_hcp_generation_widget;
		voxel_shaders_widget.hcp_voxel_genertion_selection_widget = &hcp_voxel_genertion_selection_widget;

		if (voxel_shaders_widget.voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined for voxel shader widget to perform actions on.\n");
			return;
		}

		//if (voxel_shaders_widget.voxel_hcp_generation_widget == NULL) {
		if (voxel_shaders_widget.hcp_voxel_genertion_selection_widget == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : The voxel generation widget has not been defined for the voxel shader widget to use\n");
			return;
		}

		voxel_shaders_widget.log_panel                 = log_panel;
		voxel_shaders_widget.scene_manager             = scene_manager;
		voxel_shaders_widget.current_selected_entity_id = current_selected_object_id;

		

		voxel_shaders_widget.display();
	}

	void display_voxel_automata_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id) {
		voxel_hcp_autmoata_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;

		if (voxel_hcp_autmoata_widget.voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform voxel automata actions on.\n");
			return;
		}

		voxel_hcp_autmoata_widget.log_panel                 = log_panel;
		voxel_hcp_autmoata_widget.scene_manager             = scene_manager;
		voxel_hcp_autmoata_widget.current_selected_entity_id = current_selected_object_id;

		voxel_hcp_autmoata_widget.display();
	}
/*
	void display_voxel_interactions_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id) {
		voxel_hcp_interaction_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;

		if (voxel_hcp_interaction_widget.voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform voxel interaction on.\n");
			return;
		}

		voxel_hcp_interaction_widget.log_panel = log_panel;
		voxel_hcp_interaction_widget.scene_manager = scene_manager;
		voxel_hcp_interaction_widget.current_selected_entity_id = current_selected_object_id;

		voxel_hcp_interaction_widget.display();
	}
*/
};