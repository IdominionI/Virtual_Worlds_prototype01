#pragma once

#include "../Common/definitions.h"

#include <vector>

/*
	Shader struture definitions that define the
	shader uniform variables and shader parameter
	data that is used to store shader data for
	scene data objects and rendering functions.
*/


// Compilor has linking problems with using glm vec1, ivec1 and uvec1 datatypes
// this not utilised or used !!!!
enum application_default_shader_variable_type_enum {
	none,
	Bool,
	Float1,Int1,UInt1,
	Float2,Floatv2,Int2,Intv2,UInt2,UIntv2,
	Float3,Floatv3,Int3,Intv3,UInt3,UIntv3,
	Float4,Floatv4,Int4,Intv4,UInt4,UIntv4,
	Mat2fv,Mat2x3fv,Mat2x4fv,
	Mat3fv,Mat3x2fv,Mat3x4fv,
	Mat4fv,Mat4x2fv,Mat4x3fv,
	
	//Floatv1, Intv1, UIntv1 // These may need to be deleted
};


/*
	application_default_shader_uniform_variables_struct_type

	Shader uniform varaibles are stored as a void
	pointer type pointing to the variable data value.
	By storing the data type that a void pointer is
	assigned to enables not having to have multiple
	functions for each uniform data type.

	Since there is a maximum of four varaibles for
	a unifirm variable, there are four void pointers
*/
struct application_default_shader_uniform_variables_struct_type {
	application_default_shader_variable_type_enum type; // type of data void pointer(s) is pointing to

	string_type name;
	void       *value0, *value1, *value2, *value3;  // pointer to uniform value of data type defined by type
};

/*
	Data structure types to store application real, integer 
	and boolean shader uniform variables and widget variable
	values to modify that variable.
*/
struct shader_parameter_variable_struct_type {
	string_type variable_name    = "";
	float   value                = 0.0f;
	float   variable_step        = 0.0f;
	bool    active_variable      = false;
	bool    active_variable_step = false;
	float   slider_min           = -10;
	float   slider_max           = 10;

	//bool    pass_to_voxel_geometry_shader    = false;
	bool    pass_to_geometry_shader       = false;
	bool    pass_to_point_geometry_shader = false;
	bool    pass_to_fragment_shader       = false;
};

struct shader_parameter_int_variable_struct_type {
	string_type variable_name    = "";
	int     value                = 0;
	int     variable_step        = 0;
	bool    active_variable      = false;
	bool    active_variable_step = false;
	int     slider_min           = -10;
	int     slider_max           = 10;

	//bool    pass_to_voxel_geometry_shader    = false;
	bool    pass_to_geometry_shader       = false;
	bool    pass_to_point_geometry_shader = false;
	bool    pass_to_fragment_shader       = false;
};

struct shader_parameter_bool_variable_struct_type {
	string_type variable_name = "";
	bool    value             = false;
	bool    active_variable   = false;

	//bool    pass_to_voxel_geometry_shader    = false;
	bool    pass_to_geometry_shader       = false;
	bool    pass_to_point_geometry_shader = false;
	bool    pass_to_fragment_shader       = false;
};

struct shader_display_parameters_struct_type {
	bool display_default_vertex_shader         = true;
	bool display_default_geometry_shader       = true;
	bool display_default_point_geometry_shader = true;
	bool display_default_fragment_shader       = true;
};

/*
	Data structure to store shader information and shader
	uniform variable data for a scene entity object 
*/
struct shader_parameters_struct_type {
	~shader_parameters_struct_type() { //*****
		variables.clear();
		variables.shrink_to_fit();
		int_variables.clear();
		int_variables.shrink_to_fit();
		bool_variables.clear();
		bool_variables.shrink_to_fit();
	}

	string_type  vertex_shader_file_name            = "";
	string_type  geometry_shader_file_name          = "";
	string_type  point_geometry_shader_file_name    = "";
	string_type  fragment_shader_file_name          = "";

	string_type default_vertex_shader_file_pathname   = "";
	string_type default_geometry_shader_file_pathname = "";
	string_type default_point_shader_file_pathname    = "";
	string_type default_fragment_shader_file_pathname = "";

	string_type vertex_shader_file_pathname   = "";
	string_type geometry_shader_file_pathname = "";
	string_type point_shader_file_pathname    = "";
	string_type fragment_shader_file_pathname = "";

	bool use_default_vertex_shader   = true;
	bool use_default_geometry_shader = true;
	bool use_default_point_shader    = true;
	bool use_default_fragment_shader = true;

	bool use_point_shader = true;
	bool animate_shaders  = false;

	//bool display_native_vertex_colors = false; // Remove this !!!! ??????

	shader_display_parameters_struct_type shader_display_parameters;

	int frame;
	//float    t=0.0f;

	std::vector<shader_parameter_variable_struct_type>      variables;
	std::vector<shader_parameter_int_variable_struct_type>  int_variables;
	std::vector<shader_parameter_bool_variable_struct_type> bool_variables;

	std::vector <application_default_shader_uniform_variables_struct_type> default_shader_uniform_variables;

	int get_default_shader_uniform_variable(string_type name) {
		for (int i = 0; i < default_shader_uniform_variables.size();i++) {
			application_default_shader_uniform_variables_struct_type default_shader_uniform_variable = default_shader_uniform_variables[i];
			if (default_shader_uniform_variable.name == name)
				return i;
		}

		return -1;
	}


};

