#pragma once

#include <Universal/ImGui/imgui.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <Source/Editor/Tools/dialogs.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include "../../Voxel_hcp_object/DataTypes/dt_voxel_generator.h"
#include "../../Kernal/voxel_function_import_export.h"

#include "generator_variables_widget.h"
#include "../../Compute/voxel_compute_generation.h"

#include "cart_to_voxel_generator.h"

class hcp_voxel_genertion_selection_widget_class {
public:
	hcp_voxel_genertion_selection_widget_class() {
	}

	~hcp_voxel_genertion_selection_widget_class() {}

	log_panel_class* log_panel = NULL;

	id_type                  current_selected_entity_id   = -1;  // entity id of the selected entity to display/modify
	voxel_hcp_object_class  *voxel_hcp_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
	scene_manager_class     *scene_manager = NULL;

	bool display_as_points = true;

	void display() {
		if (voxel_hcp_object_to_execute == NULL) {
			return;
		}

		float x_pos = 10.0f, y_pos = 70.0f;

		text("HCP Voxel Generation", x_pos+60, y_pos);
		
		y_pos += 25;
		text(" Matrix\nFunction", x_pos + 50, y_pos);
		text("Cartesian\nCoordinate", x_pos + 170, y_pos);
		y_pos += 40;

		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::RadioButton("###hcpvgtm", &hcp_generation_type, 0)) {
			display_cart_to_voxel_widget(voxel_hcp_object_to_execute, current_selected_entity_id);
		}

		ImGui::SetCursorPosX(x_pos + 190);
		ImGui::SetCursorPosY(y_pos);
		if(ImGui::RadioButton("###hcpvgtc", &hcp_generation_type, 1)) {
			display_voxel_generation_widget(voxel_hcp_object_to_execute, current_selected_entity_id);
		}

		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::Checkbox("Display as points###gdisplpts", &display_as_points)) {
			change_voxels_display();
		}

		y_pos += 30;
		text("Voxel Scale", x_pos + 130, y_pos);

		y_pos += 20;
		if (float_min_max_slider("v", x_pos, y_pos, 200.0f, min_vscale, max_vscale, voxel_scale_value, 50.0f)) {
			voxel_hcp_generation_widget.voxel_scale_value = voxel_scale_value;
			update_voxel_size();
		}

		if(hcp_generation_type == 0) 
			display_voxel_generation_widget(voxel_hcp_object_to_execute, current_selected_entity_id);
		else
			display_cart_to_voxel_widget(voxel_hcp_object_to_execute, current_selected_entity_id);
/*
		if (ImGui::BeginTabBar("##hcp_gen_tabs", ImGuiTabBarFlags_None))
		{
			if (hcp_generation_type == 1 && ImGui::BeginTabItem("Cartesian##cart_to_voxel_tab")) {
				display_cart_to_voxel_widget(voxel_hcp_object_to_execute, current_selected_entity_id);
				ImGui::EndTabItem();
			}

			if (hcp_generation_type== 0 && ImGui::BeginTabItem("HCP Matrix##hcp_voxel_tab")) {
				display_voxel_generation_widget(voxel_hcp_object_to_execute, current_selected_entity_id);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
*/
	}

	void change_voxels_display() {
			scene_node_class <render_object_class> *scene_voxel_object = scene_manager->get_render_object(current_selected_entity_id);

			if (scene_voxel_object != NULL && scene_manager != NULL) {
				scene_voxel_object->scene_graph_object.scene_object_class.shader_material->use_point_shader = display_as_points;

//printf("hcp_voxel_genertion_selection_widget_class :: change_voxels_display 000 : %i : %i\n", display_as_points, scene_voxel_object->scene_graph_object.scene_object_class.shader_material->use_point_shader);
				scene_manager->update_shader(scene_voxel_object, ENTITY_CATEGORY_HCP_VOXEL);

				update_voxel_size();
			}
	}

	void update_voxel_size() {
		scene_node_class <render_object_class> *scene_voxel_object = scene_manager->get_render_object(current_selected_entity_id);

		if (scene_voxel_object != NULL) {
//printf("hcp_voxel_genertion_selection_widget_class :: update_voxel_size 000 : %i : %i\n", display_as_points, scene_voxel_object->scene_graph_object.scene_object_class.shader_material->use_point_shader);

			float vox_size = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * voxel_scale_value;

			shader_class shader;

			shader.set_f1(scene_voxel_object->scene_graph_object.scene_object_class.shader_material->shader_program_id, vox_size, "voxSize");

			float voxel_hcp_z_increment = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * 2.0 * sqrt(6.0) / 3.0;

			shader.set_f1(scene_voxel_object->scene_graph_object.scene_object_class.shader_material->shader_program_id, voxel_hcp_z_increment, "voxel_hcp_z_increment");
		}
	}

private:
	int hcp_generation_type = 0;

	float voxel_scale_value = 1.0f;
	float min_vscale = 0.001f, max_vscale = 1.000f;

	cart_to_voxel_widget_class		  cart_to_voxel_widget;
	voxel_hcp_generation_widget_class voxel_hcp_generation_widget;

	void display_cart_to_voxel_widget(voxel_hcp_object_class *voxel_hcp_object_to_execute, id_type current_selected_object_id) {
		cart_to_voxel_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;

		if (cart_to_voxel_widget.voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform voxel generation procedure on.\n");
			return;
		}
//printf("voxel_hcp_generation_widget.voxel_hcp_object_to_execute != NULL ^^^^\n");
		cart_to_voxel_widget.log_panel                  = log_panel;
		cart_to_voxel_widget.scene_manager              = scene_manager;
		cart_to_voxel_widget.current_selected_entity_id = current_selected_object_id;
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget22\n");
		cart_to_voxel_widget.display();
	}

	void display_voxel_generation_widget(voxel_hcp_object_class *voxel_hcp_object_to_execute, id_type current_selected_object_id) {
		voxel_hcp_generation_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;

		if (voxel_hcp_generation_widget.voxel_hcp_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform voxel generation procedure on.\n");
			return;
		}
//printf("voxel_hcp_generation_widget.voxel_hcp_object_to_execute != NULL ^^^^\n");
		voxel_hcp_generation_widget.log_panel                  = log_panel;
		voxel_hcp_generation_widget.scene_manager              = scene_manager;
		voxel_hcp_generation_widget.current_selected_entity_id = current_selected_object_id;
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget22\n");
		voxel_hcp_generation_widget.display();
	}

};