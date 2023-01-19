#pragma once
#include <string>

//#include "../voxel_data_storage.h"

#include "../parameters.h"

/*
	Structures and definitions that define the parameters
	and variables to perform cellula automata rules on a
	HCP voxel 3D volume data, and export/import the
	parameter data.
*/

#define NUMBER_HCP_NEIGHBOURS 12

#define GENERATED_VOXELS 0
#define DEFINED_VOXELS   1
#define RANDOM_GENERATED 2

#define NEIGHBOR_IGNOR_RULE_CODE    -1
#define NEIGHBOR_INACTIVE_RULE_CODE  0
#define NEIGHBOR_ACTIVE_RULE_CODE    1

// Start New voxel automata definitions
#define NEIGHBOR_LESS_THAN_RULE_CODE           -1020
#define NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE  -1010
#define NEIGHBOR_NOT_EQUAL_RULE_CODE           -1000
#define NEIGHBOR_EQUAL_RULE_CODE                1000
#define NEIGHBOR_GREATER_THAN_OR_EQUALRULE_CODE 1010
#define NEIGHBOR_GREATER_THAN_RULE_CODE         1020

typedef unsigned long int u_long_int;

enum class lower_rule_condition_enum_type {ignore,LTEQ,LT};
enum class upper_rule_condition_enum_type {ignore,NE,EQ,LTEQ,LT};

struct hcp_automata_neighbour_rule_condition_struct_type {
	voxel_data_type lower_rule_value          = voxel_data_type(0);
	lower_rule_condition_enum_type lower_rule = lower_rule_condition_enum_type::ignore;

	upper_rule_condition_enum_type upper_rule = upper_rule_condition_enum_type::ignore;
	voxel_data_type upper_rule_value          = voxel_data_type(0);
};

struct voxel_hcp_automata_byte_rule_struct_type {
	std::string     name                = "rule";
	bool            active_rule         = false;
	voxel_data_type voxel_state         = voxel_data_type(0); // Ambition to have a function to offer calculting a value using neighbour values
	int			    start_step = 0, end_step = 0;

	hcp_automata_neighbour_rule_condition_struct_type voxel_self_rule_condition;

	hcp_automata_neighbour_rule_condition_struct_type neighbours_rule_condition[NUMBER_HCP_NEIGHBOURS];
};

// End New voxel automata definitions

//old voxel automata definitions
struct voxel_hcp_automata_rule_struct_type {
	bool       active_rule         = false;

	short      neighbours_activity[NUMBER_HCP_NEIGHBOURS];
	bool       voxel_state         = false;
	u_long_int start_step = 0, end_step = 0;

	voxel_hcp_automata_rule_struct_type(){
		for(size_t i=0;i<NUMBER_HCP_NEIGHBOURS;i++)
			neighbours_activity[i] = 0;
	}
};

struct voxel_automata_generator_parameters_struct_type {
	float	 x_start = -1.0f, x_end = 1.0f;
	float	 y_start = -1.0f, y_end = 1.0f;
	float	 z_start = -1.0f, z_end = 1.0f;
	float	 resolution_step = 0.01f;
};