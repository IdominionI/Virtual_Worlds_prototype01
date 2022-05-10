#pragma once

#include <Universal/ImGui/imgui.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>

//#include "../../Voxel_hcp_object/DataTypes/dt_voxel_automata.h"
#include "../../Hex_surface_object/DataTypes/dt_hex_surface_automata.h"

class hex_surface_automata_rules_widget_class {
public:
	//hcp_automata_rules_widget_class() {}
	//~hcp_automata_rules_widget_class() {}

	int current_selected_var_id      = -1;


	bool rules_manager(std::vector<hex_surface_automata_rule_struct_type> &automata_rules) {
		bool  value_changed = false;
		//static int selection_mask = (1 << 2);
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick; //| ImGuiTreeNodeFlags_SpanAvailWidth;

		ImGuiTreeNodeFlags node_flags = base_flags;  // Note : Critical This needs to be reinitalised for each tree node or app will crash

		if (ImGui::IsItemClicked() || ImGui::IsItemHovered()) {
			current_selected_var_id = -1;
		}

		hex_surface_automata_rule_struct_type *automata_rule;

		for (int j = 0; j < automata_rules.size(); j++) {
			automata_rule = &automata_rules[j]; // Note :: Will not work uinless real_variable is a pointer to the entry of structure data to modify

			//printf("variables_manager :: real_var_open: %s \n" , real_variable.variable_name.c_str() ); //this is the group
			char* rule_name; rule_name = automata_rule->name.data();
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			bool real_variable_selected = ImGui::TreeNodeEx((void*)(intptr_t)&automata_rule, node_flags, "");
			std::string id_prefix = "###vaur"; std::string w_id = ""; std::string w_id2 = "";

			if (ImGui::IsItemClicked() || ImGui::IsItemHovered()) {
				//current_selected_var_type_id = real_var_type_id;
				current_selected_var_id = j;
				//printf("display_nodes :: entity selected: %i  \n", current_selected_node_id); //this is the group
			}

			
			ImGui::SameLine(); ImGui::Text("%d",j);
			ImGui::SameLine();
			ImGui::SetCursorPosX(50);
			ImGui::SetNextItemWidth(75.0f);
			w_id = id_prefix + std::to_string(j * 20);
			if (ImGui::InputText(w_id.c_str(), rule_name, 20, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name)) automata_rule->name = rule_name;
			ImGui::SameLine(); w_id = "Neighbour Rules" + id_prefix + std::to_string(j * 20 + 1);
			if (ImGui::Button(w_id.c_str(), { 130,20 })) {
printf("Rule button pressed\n");
				w_id = id_prefix + std::to_string(j * 20+12);
				ImGui::OpenPopup(w_id.c_str());
			}

			w_id = id_prefix + std::to_string(j * 20 + 12);
			if (ImGui::BeginPopupModal(w_id.c_str(), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking))
				{
//printf("here\n");
					float x_pos = 0, y_pos = 25;
					std::string rid_prefix = "###vaunr"; std::string r_id = "";
					const char* lower_rule_items[] = { "-", "<", "<=" };// This cannot be defined outside this function
					const char* upper_rule_items[] = { "-", "=", "!=","<","<=" };// This cannot be defined outside this function	

					if (modal_copy) {
						for (int i = 0; i < NUMBER_HEX_NEIGHBOURS; i++) {
							neighbours_rule_condition[i] = automata_rule->neighbours_rule_condition[i];

							switch (neighbours_rule_condition[i].lower_rule) {
								case hex_lower_rule_condition_enum_type::ignore : lower_rule[i] = 0; break;
								case hex_lower_rule_condition_enum_type::LT			: lower_rule[i] = 1; break;
								case hex_lower_rule_condition_enum_type::LTEQ		: lower_rule[i] = 2; break;
							}

							switch (neighbours_rule_condition[i].upper_rule ) {
								case  hex_upper_rule_condition_enum_type::ignore: upper_rule[i] = 0; break;
								case  hex_upper_rule_condition_enum_type::EQ: upper_rule[i] = 1; break;
								case  hex_upper_rule_condition_enum_type::NE:     upper_rule[i] = 2; break;
								case  hex_upper_rule_condition_enum_type::LT:     upper_rule[i] = 3; break;
								case  hex_upper_rule_condition_enum_type::LTEQ:   upper_rule[i] = 4; break;
							}

						}
						self_rule_condition = automata_rule->hex_self_rule_condition;

						switch (self_rule_condition.lower_rule) {
							case hex_lower_rule_condition_enum_type::ignore : self_lower_rule = 0; break;
							case hex_lower_rule_condition_enum_type::LT     : self_lower_rule = 1; break;
							case hex_lower_rule_condition_enum_type::LTEQ   : self_lower_rule = 2; break;
						}

						switch (self_rule_condition.upper_rule ) {
							case  hex_upper_rule_condition_enum_type::ignore: self_upper_rule = 0; break;
							case  hex_upper_rule_condition_enum_type::EQ:     self_upper_rule = 1; break;
							case  hex_upper_rule_condition_enum_type::NE:     self_upper_rule = 2; break;
							case  hex_upper_rule_condition_enum_type::LT:     self_upper_rule = 3; break;
							case  hex_upper_rule_condition_enum_type::LTEQ:   self_upper_rule = 4; break;
						}

						modal_copy = false;
					}

					text("Automata Neighbour Rules", 30, y_pos);
					y_pos += 30;
					text("Lower Rule      Upper Rule", 50, y_pos);
					for (int i = 0; i < NUMBER_HEX_NEIGHBOURS + 1; i++) {
						///hcp_automata_neighbour_rule_condition_struct_type hcp_automata_neighbour_rule_condition = 
						if (i == 3 || i == 9) ImGui::Separator();
						//if (i == 12) { 
						//	ImGui::Text("Voxel Self Rule"); 
						//}
						
						

						if(i< NUMBER_HEX_NEIGHBOURS){
							ImGui::Text("%d", i);
							ImGui::SameLine(); ImGui::SetCursorPosX(28); r_id = rid_prefix + std::to_string(i * 10); 
							ImGui::SetNextItemWidth(40); ImGui::InputFloat(r_id.c_str(), &neighbours_rule_condition[i].lower_rule_value, 0);
							// ************************** Added increment/decrement buttons ********************************
							float t_height = ImGui::GetTextLineHeight();
							float b_height = ImGui::GetTextLineHeight() / 2.0;
							float xpos = ImGui::GetCursorPosX(); float ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i*10+1); ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
							ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
																			   // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
							if (ImGui::ButtonEx(r_id.c_str(), ImVec2(15, b_height + 3))) {
								if (neighbours_rule_condition[i].lower_rule_value < 255)
									neighbours_rule_condition[i].lower_rule_value += 1;
							}

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10 + 2); ImGui::SetNextItemWidth(15);
							ImGui::SetCursorPosY(ypos - b_height - 8);
							ImGui::SetCursorPosX(xpos + 66);

							if (ImGui::ButtonEx(r_id.c_str(), ImVec2(15, b_height + 3))) {
								if (neighbours_rule_condition[i].lower_rule_value > 0)
									neighbours_rule_condition[i].lower_rule_value -= 1;
							}
							// *************************************************************************
							//  -------- Combo box to select lower rule condition
	

							ImGui::SameLine(); ImGui::SetCursorPosY(ypos - t_height - b_height - 5); r_id = rid_prefix + std::to_string(i * 10 + 4); ImGui::SetNextItemWidth(50);
							
							ImGui::Combo(r_id.c_str(), &lower_rule[i], &Funcs::ItemGetter, lower_rule_items, IM_ARRAYSIZE(lower_rule_items));

							ImGui::SameLine(); ImGui::Text(" VV ");

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10 + 5); ImGui::SetNextItemWidth(50);

							ImGui::Combo(r_id.c_str(), &upper_rule[i], &Funcs::ItemGetter, upper_rule_items, IM_ARRAYSIZE(upper_rule_items));
							// --------------------------------------------------

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10+6); ImGui::SetNextItemWidth(40); ImGui::InputFloat(r_id.c_str(), &neighbours_rule_condition[i].upper_rule_value, 0);
							// ************************** Added increment/decrement buttons ********************************
							t_height = ImGui::GetTextLineHeight();
							b_height = ImGui::GetTextLineHeight() / 2.0;
							xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10 + 7); ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
							ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
																			   // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
							if (ImGui::ButtonEx(r_id.c_str(), ImVec2(15, b_height + 3))) {
								if (neighbours_rule_condition[i].upper_rule_value < 255)
									neighbours_rule_condition[i].upper_rule_value += 1;
							}

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10 + 8); ImGui::SetNextItemWidth(15);
							ImGui::SetCursorPosY(ypos - b_height - 8);
							ImGui::SetCursorPosX(xpos + 279);

							if (ImGui::ButtonEx(r_id.c_str(), ImVec2(15, b_height + 3))) {
								if (neighbours_rule_condition[i].upper_rule_value > 0)
									neighbours_rule_condition[i].upper_rule_value -= 1;
							}
							// *************************************************************************
							// Test for valid entries
							if (neighbours_rule_condition[i].lower_rule_value > 255) neighbours_rule_condition[i].lower_rule_value = 255;
							if (neighbours_rule_condition[i].lower_rule_value < 0)   neighbours_rule_condition[i].lower_rule_value = 0;

						}else{
							ImGui::SetCursorPosX(105); ImGui::Text("Voxel Self Rule");
							ImGui::Text("%d", i); ImGui::SameLine();
							r_id = rid_prefix + std::to_string(i); ImGui::SetNextItemWidth(40); ImGui::InputFloat(r_id.c_str(), &self_rule_condition.lower_rule_value, 0);
							// ************************** Added increment/decrement buttons ********************************
							float t_height = ImGui::GetTextLineHeight();
							float b_height = ImGui::GetTextLineHeight() / 2.0;
							float xpos = ImGui::GetCursorPosX(); float ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10 + 1); ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
							ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
																			   // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
							if (ImGui::ButtonEx(r_id.c_str(), ImVec2(15, b_height + 3))) {
								if (self_rule_condition.lower_rule_value < 255)
									self_rule_condition.lower_rule_value += 1;
							}

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10 + 2); ImGui::SetNextItemWidth(15);
							ImGui::SetCursorPosY(ypos - b_height - 8);
							ImGui::SetCursorPosX(xpos + 66);

							if (ImGui::ButtonEx(r_id.c_str(), ImVec2(15, b_height + 3))) {
								if (self_rule_condition.lower_rule_value > 0)
									self_rule_condition.lower_rule_value -= 1;
							}
							// *************************************************************************
							//  -------- Combo box to select lower rule condition

							//const char* lower_rule_items[] = { "-", "<", "<=" };// This cannot be defined outside this function	

							ImGui::SameLine(); ImGui::SetCursorPosY(ypos - t_height - b_height - 5); r_id = rid_prefix + std::to_string(i * 10 + 4); ImGui::SetNextItemWidth(50);
							
							ImGui::Combo(r_id.c_str(), &self_lower_rule, &Funcs::ItemGetter, lower_rule_items, IM_ARRAYSIZE(lower_rule_items));

							ImGui::SameLine(); ImGui::Text(" VV ");

							//const char* upper_rule_items[] = { "-", "=", "!=","<","<="};// This cannot be defined outside this function	

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10 + 5); ImGui::SetNextItemWidth(50);

							ImGui::Combo(r_id.c_str(), &self_upper_rule, &Funcs::ItemGetter, upper_rule_items, IM_ARRAYSIZE(upper_rule_items));
							// --------------------------------------------------
							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10 + 6); ImGui::SetNextItemWidth(40); ImGui::InputFloat(r_id.c_str(), &self_rule_condition.upper_rule_value, 0);
							// ************************** Added increment/decrement buttons ********************************
							t_height = ImGui::GetTextLineHeight();
							b_height = ImGui::GetTextLineHeight() / 2.0;
							xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10 + 7); ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
							ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
																			   // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
							if (ImGui::ButtonEx(r_id.c_str(), ImVec2(15, b_height + 3))) {
								if (self_rule_condition.upper_rule_value < 255)
									self_rule_condition.upper_rule_value += 1;
							}

							ImGui::SameLine(); r_id = rid_prefix + std::to_string(i * 10 + 8); ImGui::SetNextItemWidth(15);
							ImGui::SetCursorPosY(ypos - b_height - 8);
							ImGui::SetCursorPosX(xpos + 279);

							if (ImGui::ButtonEx(r_id.c_str(), ImVec2(15, b_height + 3))) {
								if (self_rule_condition.upper_rule_value > 0)
									self_rule_condition.upper_rule_value -= 1;
							}
							// *************************************************************************
							// Test for valid entries
							if (self_rule_condition.lower_rule_value > 255) self_rule_condition.lower_rule_value = 255;
							if (self_rule_condition.lower_rule_value < 0)   self_rule_condition.lower_rule_value = 0;
						}

						y_pos += 23;
					}

					y_pos += 100;

					if (ex_button("Cancel:::vaunrcb", x_pos + 10, y_pos + 30, 50, 20)) {
						modal_copy = true;
						ImGui::CloseCurrentPopup();
					}

					if (ex_button("Accept:::vaunrab", x_pos + 230, y_pos + 30, 50, 20)){
//printf("Accpept Rule button pressed\n");
						for (int i = 0; i < NUMBER_HEX_NEIGHBOURS; i++) {
							switch (lower_rule[i]) {
								case 0: neighbours_rule_condition[i].lower_rule = hex_lower_rule_condition_enum_type::ignore; break;
								case 1: neighbours_rule_condition[i].lower_rule = hex_lower_rule_condition_enum_type::LT;     break;
								case 2: neighbours_rule_condition[i].lower_rule = hex_lower_rule_condition_enum_type::LTEQ;   break;
							}

							switch (upper_rule[i]) {
								case 0: neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::ignore; break;
								case 1: neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::EQ;     break;
								case 2: neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::NE;     break;
								case 3: neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::LT;     break;
								case 4: neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::LTEQ;   break;
							}

							automata_rule->neighbours_rule_condition[i] = neighbours_rule_condition[i];
						}

						switch (self_lower_rule) {
							case 0: self_rule_condition.lower_rule = hex_lower_rule_condition_enum_type::ignore; break;
							case 1: self_rule_condition.lower_rule = hex_lower_rule_condition_enum_type::LT;     break;
							case 2: self_rule_condition.lower_rule = hex_lower_rule_condition_enum_type::LTEQ;   break;
						}

						switch (self_upper_rule) {
							case 0: self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::ignore; break;
							case 1: self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::EQ;     break;
							case 2: self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::NE;     break;
							case 3: self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::LT;     break;
							case 4: self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::LTEQ;   break;
						}

						automata_rule->hex_self_rule_condition = self_rule_condition;
						modal_copy = true;
						ImGui::CloseCurrentPopup();
					}
				ImGui::EndPopup();
			}

			text("Rule     Start    Stop   Activate",50,j*65+35);
			ImGui::SetCursorPosX(50); ImGui::SetCursorPosY(55 + j * 65);
			w_id = id_prefix + std::to_string(j * 20+2);
			//ImGui::SetNextItemWidth(40); ImGui::InputInt(w_id.c_str(), &automata_rule->voxel_state, 0);
			ImGui::SetNextItemWidth(40); ImGui::InputFloat(w_id.c_str(), &automata_rule->hex_state, 0);
			// ************************** Added increment/decrement buttons ********************************
			float t_height = ImGui::GetTextLineHeight();
			float b_height = ImGui::GetTextLineHeight() / 2.0;
			float xpos = ImGui::GetCursorPosX(); float ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

			ImGui::SameLine(); w_id = id_prefix + std::to_string(j * 20 + 3); ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
			ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
															   // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
			if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
				if (automata_rule->hex_state < 255)
					automata_rule->hex_state += 1;
			}

			ImGui::SameLine(); w_id = id_prefix + std::to_string(j * 20 + 4); ImGui::SetNextItemWidth(15);
			ImGui::SetCursorPosY(ypos - b_height - 8);
			ImGui::SetCursorPosX(xpos + 88);

			if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
				if (automata_rule->hex_state > 0)
					automata_rule->hex_state -= 1;
			}
			// *************************************************************************
			w_id = id_prefix + std::to_string(j * 20 + 5);
			ImGui::SameLine(); ImGui::SetCursorPosY(ypos - t_height - b_height - 3); ImGui::SetNextItemWidth(40); ImGui::InputInt(w_id.c_str(), &automata_rule->start_step, 0);
			// ************************** Added increment/decrement buttons ********************************
			t_height = ImGui::GetTextLineHeight();
			b_height = ImGui::GetTextLineHeight() / 2.0;
			xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

			ImGui::SameLine(); w_id = id_prefix + std::to_string(j * 20 + 6); ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
			ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
															   // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
			if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
				if (automata_rule->start_step < automata_rule->end_step)
					automata_rule->start_step += 1;
			}

			ImGui::SameLine(); w_id = id_prefix + std::to_string(j * 20 + 7); ImGui::SetNextItemWidth(15);
			ImGui::SetCursorPosY(ypos - b_height - 8);
			ImGui::SetCursorPosX(xpos + 155);

			if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
				if (automata_rule->start_step > 0)
					automata_rule->start_step -= 1;
			}
			// *************************************************************************
			w_id = id_prefix + std::to_string(j * 20 + 8);
			ImGui::SameLine(); ImGui::SetCursorPosY(ypos - t_height - b_height - 3); ImGui::SetNextItemWidth(40); ImGui::InputInt(w_id.c_str(), &automata_rule->end_step, 0);
			// ************************** Added increment/decrement buttons ********************************
			t_height = ImGui::GetTextLineHeight();
			b_height = ImGui::GetTextLineHeight() / 2.0;
			xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

			ImGui::SameLine(); w_id = id_prefix + std::to_string(j * 20 + 9); ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
			ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
															   // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
			if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
				//if (automata_rule->start_step < 255)
				automata_rule->end_step += 1;
			}

			ImGui::SameLine(); w_id = id_prefix + std::to_string(j * 20 + 10); ImGui::SetNextItemWidth(15);
			ImGui::SetCursorPosY(ypos - b_height - 8);
			ImGui::SetCursorPosX(xpos + 222);

			if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
				if (automata_rule->end_step > automata_rule->start_step)
					automata_rule->end_step -= 1;
			}
			// *************************************************************************

			ImGui::SameLine(); w_id = id_prefix + std::to_string(j * 20 + 11);
			ImGui::SetCursorPosY(ypos - t_height - b_height - 3);
			ImGui::Checkbox(w_id.c_str(), &automata_rule->active_rule);

		}
		//ImGui::TreePop();

		if (current_selected_var_id > -1) {
			//printf("current_selected_var_id : %i\n", current_selected_var_id);
			if (ImGui::BeginPopupContextWindow("vautosr1", 1, true)) {
				//if (ImGui::BeginMenu("Delete Variable")) {
				if (ImGui::MenuItem("Delete Rule")) delete_rule(current_selected_var_id, automata_rules);
				//	ImGui::EndMenu();
				//};
				ImGui::EndPopup();// Root
			}
		}
		else {
			if (current_selected_var_id < 0)
				if (ImGui::BeginPopupContextWindow("sr2", 1, true)) {
					if (ImGui::MenuItem("Add Variable")) add_rule( automata_rules);
					ImGui::EndPopup();// Root
				}
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			//current_selected_var_type_id = -1;
			current_selected_var_id = -1;
		}

		return value_changed;

	}

	void add_rule(std::vector<hex_surface_automata_rule_struct_type> &automata_rules) {
		//printf("in add variabel %i\n", var_type_id);

		hex_surface_automata_rule_struct_type automata_rule;
		automata_rule.name += std::to_string(automata_rules.size());

		automata_rules.push_back(automata_rule);
	}

	void delete_rule(int var_id, std::vector<hex_surface_automata_rule_struct_type> &automata_rules) {
		//printf("in delete variabel %i : %i\n", var_type_id, var_id);

		automata_rules.erase(automata_rules.begin() + var_id);
	}

private:
	bool modal_copy = true;

	hex_automata_neighbour_rule_condition_struct_type neighbours_rule_condition[NUMBER_HEX_NEIGHBOURS];
	int lower_rule[NUMBER_HEX_NEIGHBOURS], upper_rule[NUMBER_HEX_NEIGHBOURS];
	hex_automata_neighbour_rule_condition_struct_type self_rule_condition;
	int self_lower_rule, self_upper_rule;

	struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };
};