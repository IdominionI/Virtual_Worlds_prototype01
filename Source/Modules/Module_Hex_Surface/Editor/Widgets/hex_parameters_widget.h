#pragma once

#include <Source/Editor/Scene/scene_manager.h>

#include "generation_selection_widget.h"
#include "shaders_widget.h"
#include "automata_widget.h"

/*
				Hex surface paramters widget

	This widget class is a parent widget that manages the display
	of hex generation, shader variable and cellula automata child
	widgets via selection of ImGui tabs.

	This widget class is thus the widget class that is called by
	the main application editor to display hex surface parameter
	data to be changed or generate data.
*/

class hex_surface_parameters_widget_class {
public:
	hex_surface_parameters_widget_class() {}
	~hex_surface_parameters_widget_class() {}

	scene_manager_class *scene_manager = NULL;

	log_panel_class     *log_panel     = NULL;

	void display_hex_surface_generation_widget(id_type current_selected_object_id) {
//printf("parameter_panel_class :: display_hex_surface_generation_widget 11\n");

		if (scene_manager == NULL) {
		//	if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No scene manager defined to extract scene voxel hcp object parameter data from.\n");
			return;
		}

		if (current_selected_object_id < 0) {
		//	if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Invalid selected ID number given for the current selected hcp voxel object. No object selected\n");
			return;
		}

		//hex_surface_object_class *hex_surface_object_to_execute = scene_manager->entities_manager.get_hex_surface_entity_object(current_selected_object_id);	// Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
		hex_surface_object_class *hex_surface_object_to_execute = (hex_surface_object_class *)scene_manager->entities_manager.get_entity_of_category(current_selected_object_id, ENTITY_CATEGORY_HEX_SURF);	// Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
		
		if (hex_surface_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex surface object defined to perform tasks on.\n");
			return;
		}

		if (ImGui::BeginTabBar("##hex_surface_tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Generation##hex_generation_tab")) {
				display_hex_surface_generation_selection_widget(hex_surface_object_to_execute, current_selected_object_id);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Shader##voxel_shader_tab")){
				display_hex_surface_shader_widget(hex_surface_object_to_execute, current_selected_object_id);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Automata##voxel_auto_tab")) {
				display_hex_surface_automata_widget(hex_surface_object_to_execute, current_selected_object_id);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}



private:
	hex_surface_genertion_selection_widget_class hex_surface_genertion_selection_widget;

	hex_surface_shaders_widget_class    hex_surface_shaders_widget;
	hex_surface_autmoata_widget_class	hex_surface_autmoata_widget;


	void display_hex_surface_generation_selection_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id) {
		hex_surface_genertion_selection_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

		if (hex_surface_genertion_selection_widget.hex_surface_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex suface object defined to perform hex generation procedure on.\n");
			return;
		}
//printf("voxel_hcp_generation_widget.voxel_hcp_object_to_execute != NULL ^^^^\n");
		hex_surface_genertion_selection_widget.log_panel = log_panel;
		hex_surface_genertion_selection_widget.scene_manager = scene_manager;
		hex_surface_genertion_selection_widget.current_selected_entity_id = current_selected_object_id;
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget22\n");
		hex_surface_genertion_selection_widget.display();
	}

	void display_hex_surface_shader_widget(hex_surface_object_class *hex_surface_object_to_execute, id_type current_selected_object_id) {
		hex_surface_shaders_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;
		hex_surface_shaders_widget.hex_surface_generation_widget = &hex_surface_genertion_selection_widget;

		if (hex_surface_genertion_selection_widget.hex_surface_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex suface object defined to perform hex shader procedures on.\n");
			return;
		}

		if (hex_surface_shaders_widget.hex_surface_generation_widget == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : The hex surface generation widget has not been defined for the hex shader widget to use\n");
			return;
		}

		hex_surface_shaders_widget.log_panel                  = log_panel;
		hex_surface_shaders_widget.scene_manager              = scene_manager;
		hex_surface_shaders_widget.current_selected_entity_id = current_selected_object_id;
		
		hex_surface_shaders_widget.display();

	}

	void display_hex_surface_automata_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id) {
		hex_surface_autmoata_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

		if (hex_surface_genertion_selection_widget.hex_surface_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex suface object defined to perform hex surface automata actions on.\n");
			return;
		}

		hex_surface_autmoata_widget.log_panel                 = log_panel;
		hex_surface_autmoata_widget.scene_manager             = scene_manager;
		hex_surface_autmoata_widget.current_selected_enity_id = current_selected_object_id;

		hex_surface_autmoata_widget.display();
	}

};