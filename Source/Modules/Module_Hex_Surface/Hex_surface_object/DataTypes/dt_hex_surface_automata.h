#pragma once
#include <string>

//#include "../hex_surface_data_storage.h"

#include "../hex_surface_parameters.h"

/*
	Structures and definitions that define the parameters
	and variables to perform cellula automata rules on a
	hexagonal 2d surface data, and export/import the 
	parameter data.
*/

#define NUMBER_HEX_NEIGHBOURS 6

#define GENERATED_HEX_SURFACE 0
#define DEFINED_HEX_SURFACE   1
#define RANDOM_GENERATED      2

#define HEX_NEIGHBOR_IGNOR_RULE_CODE    -1
#define HEX_NEIGHBOR_INACTIVE_RULE_CODE  0
#define HEX_NEIGHBOR_ACTIVE_RULE_CODE    1

// Start New voxel automata definitions
#define HEX_NEIGHBOR_LESS_THAN_RULE_CODE           -2020
#define HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE  -2010
#define HEX_NEIGHBOR_NOT_EQUAL_RULE_CODE           -2000
#define HEX_NEIGHBOR_EQUAL_RULE_CODE                2000
#define HEX_NEIGHBOR_GREATER_THAN_OR_EQUALRULE_CODE 2010
#define HEX_NEIGHBOR_GREATER_THAN_RULE_CODE         2020

typedef unsigned long int u_long_int;

enum class hex_lower_rule_condition_enum_type {ignore,LTEQ,LT};
enum class hex_upper_rule_condition_enum_type {ignore,NE,EQ,LTEQ,LT};

struct hex_automata_neighbour_rule_condition_struct_type {
	hex_surface_data_type lower_rule_value        = hex_surface_data_type(0);
	hex_lower_rule_condition_enum_type lower_rule = hex_lower_rule_condition_enum_type::ignore;

	hex_upper_rule_condition_enum_type upper_rule = hex_upper_rule_condition_enum_type::ignore;
	hex_surface_data_type upper_rule_value        = hex_surface_data_type(0);
};

struct hex_surface_automata_rule_struct_type {
	std::string           name                     = "rule";
	bool                  active_rule              = false;
	hex_surface_data_type hex_state        = hex_surface_data_type(0); // Ambition to have a function to offer calculting a value using neighbour values
	int                   start_step = 0, end_step = 0;

	hex_automata_neighbour_rule_condition_struct_type hex_self_rule_condition;

	hex_automata_neighbour_rule_condition_struct_type neighbours_rule_condition[NUMBER_HEX_NEIGHBOURS];
};

// End New voxel automata definitions

struct hex_surface_automata_generator_parameters_struct_type {
	float	 x_start = -1.0f, x_end = 1.0f;
	float	 y_start = -1.0f, y_end = 1.0f;
	float	 resolution_step = 0.01f;
};

//old voxel automata definitions
/*
struct hex_surface_automata_rule_struct_type {
	bool       active_rule         = false;

	short      neighbours_activity[NUMBER_HEX_NEIGHBOURS];
	bool       hex_state         = false;
	u_long_int start_step = 0, end_step = 0;

	hex_surface_automata_rule_struct_type(){
		for(size_t i=0;i<NUMBER_HEX_NEIGHBOURS;i++)
			neighbours_activity[i] = 0;
	}
};
*/
