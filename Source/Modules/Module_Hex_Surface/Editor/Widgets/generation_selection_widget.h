#pragma once

#include <Universal/ImGui/imgui.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <Source/Editor/Tools/dialogs.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include "../../hex_surface_object/DataTypes/dt_hex_surface_generator.h"
#include "../../Kernal/hex_surface_function_import_export.h"

#include "generator_widget.h" // *****

#include "generator_variables_widget.h"
#include "../../Compute/hex_surface_compute_generation.h"

#include "cart_to_hex_generator.h"

class hex_surface_genertion_selection_widget_class {
public:
	hex_surface_genertion_selection_widget_class() {
	}

	~hex_surface_genertion_selection_widget_class() {}

	log_panel_class* log_panel = NULL;

	id_type                    current_selected_entity_id   = -1;  // entity id of the selected entity to display/modify
	hex_surface_object_class  *hex_surface_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
	scene_manager_class       *scene_manager = NULL;

	bool display_as_points = true;

	void display() {
		if (hex_surface_object_to_execute == NULL) {
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
		if (ImGui::RadioButton("###hexvgtm", &hcp_generation_type, 0)) {
			display_cart_to_hex_widget(hex_surface_object_to_execute, current_selected_entity_id);
		}

		ImGui::SetCursorPosX(x_pos + 190);
		ImGui::SetCursorPosY(y_pos);
		if(ImGui::RadioButton("###hexvgtc", &hcp_generation_type, 1)) {
			display_hex_surface_generation_widget(hex_surface_object_to_execute, current_selected_entity_id);
		}

		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::Checkbox("Display as points###hgdisplpts", &display_as_points)) {
			change_hex_display();
		}

		y_pos += 30;
		text("Hex Scale", x_pos + 130, y_pos);

		y_pos += 20;

		if (float_min_max_slider("v", x_pos, y_pos, 200.0f, min_vscale, max_vscale, hex_scale_value, 50.0f)) {
			hex_surface_generation_widget.hex_scale_value = hex_scale_value;
			update_hex_size();// needs testing 
		}

		if(hcp_generation_type == 0) 
			display_hex_surface_generation_widget(hex_surface_object_to_execute, current_selected_entity_id);
		else
			display_cart_to_hex_widget(hex_surface_object_to_execute, current_selected_entity_id);

	}

	void change_hex_display() {
		scene_node_class <render_object_class>* scene_hex_surface_object = scene_manager->get_render_object(current_selected_entity_id);

		if (scene_hex_surface_object != NULL && scene_manager != NULL) {
			scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material->use_point_shader = display_as_points;

			shader_class shader;
			shader.set_b1(scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material->shader_program_id, display_as_points, "display_as_points");

			scene_manager->update_shader(scene_hex_surface_object, ENTITY_CATEGORY_HEX_SURF);

			update_hex_size();// *****
		}
	}

	void update_hex_size() {
		scene_node_class <render_object_class>* scene_hex_surface_object = scene_manager->get_render_object(current_selected_entity_id);

		// This method of using a unversal array of unform variables that can be used between classes or modules or funtions etc has been found to be no
		// better than just a direct use of updating uniforms through manual manipulation of widgets that change uniform variable values !!!!
/*		if (scene_hex_surface_object != NULL) {
			int hex_size_index = scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material.get_default_shader_uniform_variable("hexSize");
			if (hex_size_index < 0) {
				application_default_shader_uniform_variables_struct_type uniform_variable;
				float *vox_size = new float;
				*vox_size = hex_surface_object_to_execute->hex_surface_object_data.hex_size* hex_scale_value;
				uniform_variable.type = application_default_shader_variable_type_enum::Float1; uniform_variable.name = "hexSize"; uniform_variable.value0 = vox_size;
				scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material.default_shader_uniform_variables.push_back(uniform_variable);
printf("update_hex_size : created hexSize\n");
			} else {
				float vox_size = hex_surface_object_to_execute->hex_surface_object_data.hex_size * hex_scale_value;
				*(float*)scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material.default_shader_uniform_variables[hex_size_index].value0 = vox_size;
printf("update_hex_size : created hexSize %i : %f : %f\n", hex_size_index, vox_size, *(float*)scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material.default_shader_uniform_variables[hex_size_index].value0);

			}
		}
*/

		if (scene_hex_surface_object != NULL) {
			float vox_size = hex_surface_object_to_execute->hex_surface_object_data.hex_size * hex_scale_value;

			shader_class shader;
			shader.set_f1(scene_hex_surface_object->scene_graph_object.scene_object_class.shader_material->shader_program_id, vox_size, "hexSize");
		}
	}

private:
	int hcp_generation_type = 0;

	float hex_scale_value = 1.0f;
	float min_vscale = 0.001f, max_vscale = 1.000f;

	cart_to_hex_widget_class            cart_to_hex_widget;
	hex_surface_generation_widget_class hex_surface_generation_widget;

	void display_cart_to_hex_widget(hex_surface_object_class *hex_surface_object_to_execute, id_type current_selected_object_id) {
		cart_to_hex_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

		if (cart_to_hex_widget.hex_surface_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex suface object defined to perform hex generation procedure on.\n");
			return;
		}
//printf("voxel_hcp_generation_widget.voxel_hcp_object_to_execute != NULL ^^^^\n");
		cart_to_hex_widget.log_panel                  = log_panel;
		cart_to_hex_widget.scene_manager              = scene_manager;
		cart_to_hex_widget.current_selected_entity_id = current_selected_object_id;
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget22\n");
		cart_to_hex_widget.display();
	}

	void display_hex_surface_generation_widget(hex_surface_object_class *hex_surface_object_to_execute, id_type current_selected_object_id) {

		hex_surface_generation_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

		if (hex_surface_generation_widget.hex_surface_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex suface object defined to perform hex generation procedure on.\n");
			return;
		}

		hex_surface_generation_widget.log_panel                  = log_panel;
		hex_surface_generation_widget.scene_manager              = scene_manager;
		hex_surface_generation_widget.current_selected_entity_id = current_selected_object_id;

		hex_surface_generation_widget.display();
	}

};