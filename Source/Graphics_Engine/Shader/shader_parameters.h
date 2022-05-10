#pragma once

#include "../Common/definitions.h"

//#include <string>
#include <vector>

// Compilor has linking problems with using glm vec1, ivec1 and uvec1 datatypes
// this not utilised or used !!!!
enum application_default_shader_variable_type_enum {
	Bool,
	Float1,Int1,UInt1,
	Float2,Floatv2,Int2,Intv2,UInt2,UIntv2,
	Float3,Floatv3,Int3,Intv3,UInt3,UIntv3,
	Float4,Floatv4,Int4,Intv4,UInt4,UIntv4,
	Mat2fv,Mat2x3fv,Mat2x4dv,Mat2x4fv,
	Mat3fv,Mat3x2fv,Mat3x4fv,
	Mat4dv,Mat4fv,Mat4x2fv,Mat4x3dv,Mat4x3fv,
	
	//Floatv1, Intv1, UIntv1 // These may need to be deleted
};
//template <typename T>
struct application_default_shader_uniform_variables_struct_type {
	application_default_shader_variable_type_enum type;

	string_type name;
	void       *value0, *value1, *value2, *value3;
};


struct shader_parameter_variable_struct_type {
	string_type variable_name = "";
	float   value = 0.0f;
	float   variable_step = 0.0f;
	bool    active_variable = false;
	bool    active_variable_step = false;
	float   slider_min = -10;
	float   slider_max = 10;

	//bool    pass_to_voxel_geometry_shader    = false;
	bool    pass_to_geometry_shader = false;
	bool    pass_to_point_geometry_shader = false;
	bool    pass_to_fragment_shader = false;
};

struct shader_parameter_int_variable_struct_type {
	string_type variable_name = "";
	int     value = 0;
	int     variable_step = 0;
	bool    active_variable = false;
	bool    active_variable_step = false;
	int     slider_min = -10;
	int     slider_max = 10;

	//bool    pass_to_voxel_geometry_shader    = false;
	bool    pass_to_geometry_shader = false;
	bool    pass_to_point_geometry_shader = false;
	bool    pass_to_fragment_shader = false;
};

struct shader_parameter_bool_variable_struct_type {
	string_type variable_name = "";
	bool    value = false;
	bool    active_variable = false;

	//bool    pass_to_voxel_geometry_shader    = false;
	bool    pass_to_geometry_shader = false;
	bool    pass_to_point_geometry_shader = false;
	bool    pass_to_fragment_shader = false;
};

struct shader_display_parameters_struct_type {
	bool display_default_vertex_shader = true;
	bool display_default_geometry_shader = true;
	bool display_default_point_geometry_shader = true;
	bool display_default_fragment_shader = true;
};

struct shader_parameters_struct_type {
	//~shader_parameters_struct_type() {
	//	variables.clear(); variables.shrink_to_fit();
	//	int_variables.clear(); int_variables.shrink_to_fit();
	//	bool_variables.clear(); bool_variables.shrink_to_fit();
	//}

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

/*
	bool change_variable_value(string_type variable_name, float value) {
		// Do not use a foreach loop to change variable values as the changes are only local and will not 
		// carry over outside the loop unless changes are made to pointers or pointer content
		for (int i = 0; i < variables.size(); i++) {
			if (variables[i].variable_name == variable_name) {
				variables[i].value = value;
				//QMessageBox::information(0, "Function Expression Success", variables[i].variable_name + "  changed : "+string_type::number(value)+":"+string_type::number(variables[i].variable), QMessageBox::Ok); // testing only;
				return true;
			}
		}

		return false;
	}

	float get_variable_value(string_type variable_name) {
		//foreach(shader_parameter_variable_struct_type variable, variables) {
		for (shader_parameter_variable_struct_type variable : variables) {
			//QMessageBox::information(NULL, "", "in restore_generator_step_action_value 05 : "+variable_name +"::" + variable.variable_name+"::", QMessageBox::Ok);
			if (variable.variable_name == variable_name) {
				return variable.value;
			}
		}
		return NAN;
	}

	int get_int_variable_value(string_type variable_name) {
		for (shader_parameter_int_variable_struct_type int_variable : int_variables) {
			//QMessageBox::information(NULL, "", "in restore_generator_step_action_value 05 : "+variable_name +"::" + variable.variable_name+"::", QMessageBox::Ok);
			if (int_variable.variable_name == variable_name) {
				return int_variable.value;
			}
		}
		return NAN;
	}

	bool change_bool_variable_value(string_type variable_name, bool value) {
		// Do not use a foreach loop to change variable values as the changes are only local and will not 
		// carry over outside the loop unless changes are made to pointers or pointer content
		for (int i = 0; i < bool_variables.size(); i++) {
			if (bool_variables[i].variable_name == variable_name) {
				bool_variables[i].value = value;
				//QMessageBox::information(0, "Function Expression Success", variables[i].variable_name + "  changed : "+string_type::number(value)+":"+string_type::number(variables[i].variable), QMessageBox::Ok); // testing only;
				return true;
			}
		}

		return false;
	}

	bool get_bool_variable_value(string_type variable_name, bool& got_bool_variable) {
		for (shader_parameter_bool_variable_struct_type bool_variable : bool_variables) {
			//QMessageBox::information(NULL, "", "in restore_generator_step_action_value 05 : "+variable_name +"::" + variable.variable_name+"::", QMessageBox::Ok);
			if (bool_variable.variable_name == variable_name) {
				got_bool_variable = true;
				return bool_variable.value;
			}
		}
		got_bool_variable = false;
		return false;
	}
	*/
};

