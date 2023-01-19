#include "hcp_voxel_parameters_widget.h"

// Including any of the following generates linking errors regarding "imgui_custom_definitions.h" unkown why
//#include "generation_selection_widget.h"
//#include "shaders_widget.h"
//#include "automata_widget.h"

void voxel_hcp_parameters_widget_class::display_parameters_widget(id_type current_selected_object_id) {
	//printf("parameter_panel_class :: display_voxel_hcp_generation_widget 11\n");

	if (scene_manager == NULL) {
		//	if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No scene manager defined to extract scene voxel hcp object parameter data from.\n");
		return;
	}

	if (current_selected_object_id < 0) {
		//	if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Invalid selected ID number given for the current selected hcp voxel object. No object selected\n");
		return;
	}

	voxel_hcp_object_class* voxel_hcp_object_to_execute = (voxel_hcp_object_class*)scene_manager->entities_manager.get_entity_of_category(current_selected_object_id, ENTITY_CATEGORY_HCP_VOXEL);	// Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	if (voxel_hcp_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform tasks on.\n");
		return;
	}

	if (ImGui::BeginTabBar("##hcp_voxel_tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Generation##hcp_voxel_tab")) {
			display_voxel_generation_widget(voxel_hcp_object_to_execute, current_selected_object_id);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Shader##voxel_shader_tab")) {
			display_voxel_shader_widget(voxel_hcp_object_to_execute, current_selected_object_id);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Automata##voxel_auto_tab")) {
			display_voxel_automata_widget(voxel_hcp_object_to_execute, current_selected_object_id);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

void voxel_hcp_parameters_widget_class::display_voxel_generation_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id) {
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

void voxel_hcp_parameters_widget_class::display_voxel_shader_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id) {
	voxel_shaders_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;
	//voxel_shaders_widget.voxel_hcp_generation_widget = &voxel_hcp_generation_widget;
	voxel_shaders_widget.hcp_voxel_genertion_selection_widget = &hcp_voxel_genertion_selection_widget;

	if (voxel_shaders_widget.voxel_hcp_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined for voxel shader widget to perform actions on.\n");
		return;
	}

	if (voxel_shaders_widget.hcp_voxel_genertion_selection_widget == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : The voxel generation widget has not been defined for the voxel shader widget to use\n");
		return;
	}

	voxel_shaders_widget.log_panel = log_panel;
	voxel_shaders_widget.scene_manager = scene_manager;
	voxel_shaders_widget.current_selected_entity_id = current_selected_object_id;

	voxel_shaders_widget.display();

}

void voxel_hcp_parameters_widget_class::display_voxel_automata_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id) {
	voxel_hcp_automata_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;

	if (voxel_hcp_automata_widget.voxel_hcp_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform voxel automata actions on.\n");
		return;
	}

	voxel_hcp_automata_widget.log_panel = log_panel;
	voxel_hcp_automata_widget.scene_manager = scene_manager;
	voxel_hcp_automata_widget.current_selected_entity_id = current_selected_object_id;

	voxel_hcp_automata_widget.automata_functions_logging(log_panel);

	voxel_hcp_automata_widget.display();

}