#pragma once

#include <vector>

#include "../../Voxel_hcp_object/DataTypes/dt_voxel_automata.h"

/*
			HCP Voxel cellula automata rules widget class

	This class widget defines an ImGui widget that is used to define
	a set of cellula automata rules that the hcp voxel volume matrix
	model can be subjected to perform.
*/

class hcp_automata_rules_widget_class {
public:
	int current_selected_var_id      = -1;

	bool rules_manager(std::vector<voxel_hcp_automata_byte_rule_struct_type>& automata_rules);
	void add_rule(std::vector<voxel_hcp_automata_byte_rule_struct_type>& automata_rules);
	void delete_rule(int var_id, std::vector<voxel_hcp_automata_byte_rule_struct_type>& automata_rules);

private:
	bool modal_copy = true;

	hcp_automata_neighbour_rule_condition_struct_type neighbours_rule_condition[NUMBER_HCP_NEIGHBOURS];
	int lower_rule[NUMBER_HCP_NEIGHBOURS], upper_rule[NUMBER_HCP_NEIGHBOURS];
	hcp_automata_neighbour_rule_condition_struct_type self_rule_condition;
	int self_lower_rule, self_upper_rule;

	struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };
};