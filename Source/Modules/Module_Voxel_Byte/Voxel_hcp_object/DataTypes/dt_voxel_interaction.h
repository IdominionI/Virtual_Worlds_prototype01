#pragma once

#include <string>

#include "../voxel_hcp_object.h"

/*
		Structures of to be used in a possible future functionality
		of HCP voxel volumes interacting with each other

		No implemented or used. May change or not even be used !!!!!
*/

struct voxel_interaction_function_struct_type {
	int     interaction_id          = 0;
	std::string interaction_name        = "interaction";    // used for UI
	std::string interaction_description = "new interaction";// used for UI

	void (*interaction_function) (index_struct_type  voxel_a_index, voxel_data_type a, voxel_hcp_object_class *voxel_object_a, index_struct_type voxel_b_index, voxel_data_type b, voxel_hcp_object_class *voxel_object_b) = NULL;
};

struct voxel_interaction_struct_type : public voxel_interaction_function_struct_type {
	bool active_rule = false;
	int  voxel_a_category = 0, voxel_b_category = 0;

	int start_step = 0, end_step = -1;
};
