#include "hex_parameters_widget.h"

// Including any of the following generates linking errors regarding "imgui_custom_definitions.h" unkown why
//#include "generation_selection_widget.h"
//#include "shaders_widget.h"
//#include "automata_widget.h"

void hex_surface_parameters_widget_class::display_parameters_widget(id_type current_selected_object_id) {
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
	hex_surface_object_class* hex_surface_object_to_execute = (hex_surface_object_class*)scene_manager->entities_manager.get_entity_of_category(current_selected_object_id, ENTITY_CATEGORY_HEX_SURF);	// Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

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

		if (ImGui::BeginTabItem("Shader##voxel_shader_tab")) {
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
//-----------------------------------------------

void hex_surface_parameters_widget_class::display_hex_surface_generation_selection_widget(hex_surface_object_class * hex_surface_object_to_execute, id_type current_selected_object_id) {
	hex_surface_generation_selection_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

	if (hex_surface_generation_selection_widget.hex_surface_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex suface object defined to perform hex generation procedure on.\n");
		return;
	}
//printf("voxel_hcp_generation_widget.voxel_hcp_object_to_execute != NULL ^^^^\n");
	hex_surface_generation_selection_widget.log_panel = log_panel;
	hex_surface_generation_selection_widget.scene_manager = scene_manager;
	hex_surface_generation_selection_widget.current_selected_entity_id = current_selected_object_id;
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget22\n");
	hex_surface_generation_selection_widget.display();
}

void hex_surface_parameters_widget_class::display_hex_surface_shader_widget(hex_surface_object_class * hex_surface_object_to_execute, id_type current_selected_object_id) {
	hex_surface_shaders_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;
	hex_surface_shaders_widget.hex_surface_generation_widget = &hex_surface_generation_selection_widget;

	if (hex_surface_generation_selection_widget.hex_surface_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex suface object defined to perform hex shader procedures on.\n");
		return;
	}

	if (hex_surface_shaders_widget.hex_surface_generation_widget == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : The hex surface generation widget has not been defined for the hex shader widget to use\n");
		return;
	}

	hex_surface_shaders_widget.log_panel = log_panel;
	hex_surface_shaders_widget.scene_manager = scene_manager;
	hex_surface_shaders_widget.current_selected_entity_id = current_selected_object_id;

	hex_surface_shaders_widget.display();

}

void hex_surface_parameters_widget_class::display_hex_surface_automata_widget(hex_surface_object_class * hex_surface_object_to_execute, id_type current_selected_object_id) {
	hex_surface_automata_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

	//if (hex_surface_generation_selection_widget.hex_surface_object_to_execute == NULL) {
	if (hex_surface_automata_widget.hex_surface_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex suface object defined to perform hex surface automata actions on.\n");
		return;
	}

	hex_surface_automata_widget.log_panel = log_panel;
	hex_surface_automata_widget.scene_manager = scene_manager;
	hex_surface_automata_widget.current_selected_enity_id = current_selected_object_id;

	hex_surface_automata_widget.display();

}
