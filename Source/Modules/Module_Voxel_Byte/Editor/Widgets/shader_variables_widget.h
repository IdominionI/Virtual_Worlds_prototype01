#pragma once

#include <Universal/ImGui/imgui.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>

#include <Source/Graphics_Engine/Shader/shader_parameters.h>

/*
		HCP voxel volume shader variables widget class

	This class widget defines an ImGui widget and child widgets
	that are used to define shader variables required to be used
	in the OpenGl glsl shader program code that is compiled to
	display HCP voxel volume data.

	This class widget has inputs that define incremental step
	values that each varaible can be changed by enabling the user
	to perform incremental step changes to the shader values that
	display the hex surface interactively.
*/

class shader_variables_widget_class {
public:
	shader_variables_widget_class() {
	}
	
	~shader_variables_widget_class() {
	}

	int current_selected_var_type_id = -1;
	int current_selected_var_id      = -1;

	int real_var_type_id = 1;
	int int_var_type_id  = 2;
	int bool_var_type_id = 3;

	bool variables_manager(shader_parameters_struct_type *voxel_shader_parameters) {
		bool  value_changed = false;
		static int selection_mask = (1 << 2);
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick; //| ImGuiTreeNodeFlags_SpanAvailWidth;
		int number_variable_types = 3;

		ImGui::SetCursorPosX(10.0f); ImGui::Text("Name    vis sel   Description");

		shader_parameter_variable_struct_type      *real_variable = NULL;
		shader_parameter_int_variable_struct_type  *int_variable  = NULL;
		shader_parameter_bool_variable_struct_type *bool_variable = NULL;

		bool variables_open = ImGui::TreeNodeEx((void*)(intptr_t)0, base_flags, "Shader Variables");

		if (variables_open) {
			ImGuiTreeNodeFlags node_flags = base_flags;  // Note : Critical This needs to be reinitalised for each tree node or app will crash

			const bool is_selected = (selection_mask & (1 << number_variable_types)) != 0;

			if (is_selected)
				node_flags |= ImGuiTreeNodeFlags_Selected;

			bool real_var_open = ImGui::TreeNodeEx((void*)(intptr_t)real_var_type_id, node_flags, "Real Variables");

			if (real_var_open) {
				if (ImGui::IsItemClicked() || ImGui::IsItemHovered()) {
					current_selected_var_type_id = real_var_type_id;
					current_selected_var_id = -1;
				}

				for (int j = 0; j < voxel_shader_parameters->variables.size(); j++) {
					real_variable = &voxel_shader_parameters->variables[j]; // Note :: Will not work uinless real_variable is a pointer to the entry of structure data to modify

					char* variable_name; variable_name = real_variable->variable_name.data();
//printf("variables_manager :: real_var_open: %s \n" , real_variable.variable_name.c_str() ); //this is the group

					node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

					bool real_variable_selected = ImGui::TreeNodeEx((void*)(intptr_t)&real_variable, node_flags, "");
					std::string id_prefix = "###srv"; std::string w_id = "";

					if (ImGui::IsItemClicked() || ImGui::IsItemHovered()) {
						current_selected_var_type_id = real_var_type_id;
						current_selected_var_id = j;
//printf("display_nodes :: entity selected: %i  \n", current_selected_node_id); //this is the group
					}

					ImGui::SameLine(); ImGui::SetCursorPosX(50);
					w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10); ImGui::Checkbox(w_id.c_str(), &real_variable->active_variable);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(75.0f);
					w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 +1);
					if (ImGui::InputText(w_id.c_str(), variable_name, 20, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name)) real_variable->variable_name = variable_name;

					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 + 2); ImGui::SetNextItemWidth(50);ImGui::InputFloat(w_id.c_str(), &real_variable->value);
					// ************************** Added increment/decrement buttons ********************************
					float t_height = ImGui::GetTextLineHeight();
					float b_height = ImGui::GetTextLineHeight() / 2.0;
					float xpos = ImGui::GetCursorPosX(); float ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 + 8); ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
					ImGui::SetCursorPosY(ypos - t_height- b_height-5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
																	   // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
					if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
						if (real_variable->value + real_variable->variable_step > real_variable->slider_max)
							real_variable->value = real_variable->slider_max;
						else
							real_variable->value += real_variable->variable_step;
					}

					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 + 9); ImGui::SetNextItemWidth(15); 
					ImGui::SetCursorPosY(ypos - b_height -8);
					ImGui::SetCursorPosX(xpos + 153);

					if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
						if (real_variable->value - real_variable->variable_step < real_variable->slider_min)
							real_variable->value = real_variable->slider_min;
						else
							real_variable->value -= real_variable->variable_step;
					}
					
					// ***************************************************
					ImGui::SameLine(); ImGui::SetCursorPosY(ypos- t_height - b_height -3 ); // remove ImGui::SetCursorPosY if not using inc/dec buttons above
					w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 + 3); ImGui::SetNextItemWidth(50);ImGui::InputFloat(w_id.c_str(), &real_variable->variable_step);
					
					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10+4); ImGui::Checkbox(w_id.c_str(), &real_variable->active_variable_step);
					// min max slider widget
					ImGui::SetCursorPosX(50);
					w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 + 5); ImGui::SetNextItemWidth(50);ImGui::InputFloat(w_id.c_str(), &real_variable->slider_min);
					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 + 6); ImGui::SetNextItemWidth(150);
					if(ImGui::SliderFloat(w_id.c_str(), &real_variable->value, real_variable->slider_min, real_variable->slider_max, "%.3f", ImGuiSliderFlags_None)) value_changed = true;
					
					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 + 7); ImGui::SetNextItemWidth(50); ImGui::InputFloat(w_id.c_str(), &real_variable->slider_max);
				}
				ImGui::TreePop();
			}

			node_flags = base_flags;  // Note : Critical This needs to be reinitalised for each tree node or app will crash

			bool int_var_open = ImGui::TreeNodeEx((void*)(intptr_t)int_var_type_id, node_flags, "Integer Variables");
			if (int_var_open) {
				if (ImGui::IsItemClicked() || ImGui::IsItemHovered()) {
					current_selected_var_type_id = int_var_type_id;
					current_selected_var_id = -1;
//printf("display_nodes :: group selected: %i  \n", current_selected_node_id); //this is the group
				}

				for (int j = 0; j < voxel_shader_parameters->int_variables.size(); j++) {
					int_variable = &voxel_shader_parameters->int_variables[j]; // Note :: Will not work uinless real_variable is a pointer to the entry of structure data to modify

//printf("variables_manager :: real_var_open: %s \n" , real_variable.variable_name.c_str() ); //this is the group
					char* variable_name; variable_name = int_variable->variable_name.data();
					node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

					bool real_variable_selected = ImGui::TreeNodeEx((void*)(intptr_t)&int_variable, node_flags, "");
					std::string id_prefix = "###siv"; std::string w_id = "";

					if (ImGui::IsItemClicked() || ImGui::IsItemHovered()) {
						current_selected_var_type_id = int_var_type_id;
						current_selected_var_id = j;
//printf("display_nodes :: entity selected: %i  \n", current_selected_node_id); //this is the group
					}

					ImGui::SameLine(); ImGui::SetCursorPosX(50);
					w_id = id_prefix + std::to_string(real_var_type_id * 200 + j * 10); ImGui::Checkbox(w_id.c_str(), &int_variable->active_variable);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(75.0f);
					w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 + 1);
					if (ImGui::InputText(w_id.c_str(), variable_name, 20, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name)) int_variable->variable_name = variable_name;

					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 200 + j * 10 + 2); ImGui::SetNextItemWidth(50); ImGui::InputInt(w_id.c_str(), &int_variable->value,0);
					// ************************** Added increment/decrement buttons ********************************
					float t_height = ImGui::GetTextLineHeight();
					float b_height = ImGui::GetTextLineHeight() / 2.0;
					float xpos = ImGui::GetCursorPosX(); float ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 + 8); ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
					ImGui::SetCursorPosY(ypos - t_height- b_height-5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
																	   // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
					if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
						printf("Button + clicked\n");
						if (int_variable->value + int_variable->variable_step > int_variable->slider_max)
							int_variable->value = int_variable->slider_max;
						else
							int_variable->value += int_variable->variable_step;
					}

					ImGui::SameLine(); w_id = id_prefix + std::to_string(int_var_type_id * 100 + j * 10 + 9); ImGui::SetNextItemWidth(15); 
					ImGui::SetCursorPosY(ypos - b_height -8);
					ImGui::SetCursorPosX(xpos + 153);

					if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
						printf("Button + clicked\n");
						if (int_variable->value - int_variable->variable_step < int_variable->slider_min)
							int_variable->value = int_variable->slider_min;
						else
							int_variable->value -= int_variable->variable_step;
					}
					
					// ***************************************************
					ImGui::SameLine(); ImGui::SetCursorPosY(ypos- t_height - b_height -3 ); // remove ImGui::SetCursorPosY if not using inc/dec buttons above
					w_id = id_prefix + std::to_string(real_var_type_id * 200 + j * 10 + 3); ImGui::SetNextItemWidth(50); ImGui::InputInt(w_id.c_str(), &int_variable->variable_step,0);

					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 200 + j * 10 + 4); ImGui::Checkbox(w_id.c_str(), &int_variable->active_variable_step);
					// min max slider widget
					ImGui::SetCursorPosX(50);
					w_id = id_prefix + std::to_string(int_var_type_id * 100 + j * 10 + 5); ImGui::SetNextItemWidth(50); ImGui::InputInt(w_id.c_str(), &int_variable->slider_min,0);
					ImGui::SameLine(); w_id = id_prefix + std::to_string(int_var_type_id * 100 + j * 10 + 6); ImGui::SetNextItemWidth(150);
					if(ImGui::SliderInt(w_id.c_str(), &int_variable->value, int_variable->slider_min, int_variable->slider_max)) value_changed = true;

					ImGui::SameLine(); w_id = id_prefix + std::to_string(int_var_type_id * 100 + j * 10 + 7); ImGui::SetNextItemWidth(50); ImGui::InputInt(w_id.c_str(), &int_variable->slider_max,0);
				}
				ImGui::TreePop();
			}

			node_flags = base_flags;  // Note : Critical This needs to be reinitalised for each tree node or app will crash

			bool bool_var_open = ImGui::TreeNodeEx((void*)(intptr_t)bool_var_type_id, node_flags, "Boolean Variables");
			if (bool_var_open) {
				if (ImGui::IsItemClicked() || ImGui::IsItemHovered()) {
					current_selected_var_type_id = bool_var_type_id;
					current_selected_var_id = -1;
//printf("display_nodes :: group selected: %i  \n", current_selected_node_id); //this is the group
				}

				for (int j = 0; j < voxel_shader_parameters->bool_variables.size(); j++) {
					bool_variable = &voxel_shader_parameters->bool_variables[j]; // Note :: Will not work uinless real_variable is a pointer to the entry of structure data to modify

					char *variable_name; variable_name = bool_variable->variable_name.data();
//printf("variables_manager :: real_var_open: %s \n" , real_variable.variable_name.c_str() ); //this is the group

					node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

					bool real_variable_selected = ImGui::TreeNodeEx((void*)(intptr_t)&bool_variable, node_flags, "");
					std::string id_prefix = "###sbv"; std::string w_id = "";

					if (ImGui::IsItemClicked() || ImGui::IsItemHovered()) {
						current_selected_var_type_id = bool_var_type_id;
						current_selected_var_id = j;
//printf("display_nodes :: entity selected: %i  \n", current_selected_node_id); //this is the group
					}

					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 200 + j * 10); ImGui::Checkbox(w_id.c_str(), &bool_variable->active_variable);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(75.0f);
					w_id = id_prefix + std::to_string(real_var_type_id * 100 + j * 10 + 1);
					if (ImGui::InputText(w_id.c_str(), variable_name, 20, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name)) bool_variable->variable_name = variable_name;

					ImGui::SameLine(); w_id = id_prefix + std::to_string(real_var_type_id * 200 + j * 10 + 4); 
					if(ImGui::Checkbox(w_id.c_str(), &bool_variable->value))value_changed = true;
				}
				ImGui::TreePop();
			}

			if (current_selected_var_type_id > -1 && current_selected_var_id > -1) {
//printf("current_selected_var_id : %i\n", current_selected_var_id);
				if (ImGui::BeginPopupContextWindow("sr1", 1, true)) {
					if (ImGui::MenuItem("Delete Variable2")) delete_variable(current_selected_var_type_id, current_selected_var_id, voxel_shader_parameters);
					ImGui::EndPopup();// Root
				}
			} else{
				if (current_selected_var_type_id > -1 && current_selected_var_id < 0)
					if (ImGui::BeginPopupContextWindow("sr2", 1, true)) {
							if (ImGui::MenuItem("Add Variable")) add_variable(current_selected_var_type_id, voxel_shader_parameters);
						ImGui::EndPopup();// Root
					}
				}
// printf("real var\n");
			ImGui::TreePop();
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			current_selected_var_type_id = -1;
			current_selected_var_id      = -1;
		}

		return value_changed;
//printf("display_nodes :: entity selected: %i %i \n", current_selected_var_type_id, current_selected_var_id);
	}

	void add_variable(int var_type_id, shader_parameters_struct_type *voxel_shader_parameters) {
//printf("in add variabel %i\n", var_type_id);

		// Switch cannot use var_type_id so need to rvert to if statements !!!!!!

		if (var_type_id == real_var_type_id) {
			shader_parameter_variable_struct_type real_var;
			real_var.variable_name += std::to_string(voxel_shader_parameters->variables.size());
			voxel_shader_parameters->variables.push_back(real_var);
			return;
		}

		if (var_type_id == int_var_type_id) {
			shader_parameter_int_variable_struct_type int_var;
			int_var.variable_name += std::to_string(voxel_shader_parameters->int_variables.size());

			voxel_shader_parameters->int_variables.push_back(int_var);
			return;
		}

		if (var_type_id == bool_var_type_id) {
			shader_parameter_bool_variable_struct_type bool_var;
			bool_var.variable_name += std::to_string(voxel_shader_parameters->bool_variables.size());

			voxel_shader_parameters->bool_variables.push_back(bool_var);
			return;
		}
	}

	void delete_variable(int var_type_id,int var_id,shader_parameters_struct_type *voxel_shader_parameters) {
//printf("in delete variabel %i : %i\n", var_type_id, var_id);

		if (var_type_id == real_var_type_id) {
			voxel_shader_parameters->variables.erase(voxel_shader_parameters->variables.begin()+ var_id);
			return;
		}

		if (var_type_id == int_var_type_id) {
			voxel_shader_parameters->int_variables.erase(voxel_shader_parameters->int_variables.begin() + var_id);
			return;
		}

		if (var_type_id == bool_var_type_id) {
			voxel_shader_parameters->bool_variables.erase(voxel_shader_parameters->bool_variables.begin() + var_id);
			return;
		}

	}

private:


};