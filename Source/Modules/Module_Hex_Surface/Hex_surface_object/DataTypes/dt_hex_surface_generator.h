#pragma once

#include <string>
#include <vector>

struct hex_surface_generator_parameter_variable_struct_type {
	std::string variable_name;
	float   value;
	float   variable_step;
	bool    active_variable;
	bool    active_variable_step;
};

struct hex_surface_generator_parameter_int_variable_struct_type {
	std::string variable_name = "variable";
	int     value = 0;
	int     variable_step = 0;
	bool    active_variable      = false;
	bool    active_variable_step = false;
};

struct hex_surface_generator_parameter_bool_variable_struct_type {
	std::string variable_name   = "variable";
	bool    value           = false;
	bool    active_variable = false;
};

struct hex_surface_generator_function_variable_struct_type {
	std::string variable_name;
	float* variable;

	hex_surface_generator_function_variable_struct_type() {
		variable = new float;
		*variable = 0.0f;
	}
};

struct hex_generator_display_struct_type {
	bool  display_as_points = false;
	float voxel_scale = 1000.0;
	//float voxel_scale       = 1.0;
	float min_scale_value = 1.0, max_scale_value = 1000.0;
};


struct hex_surface_generator_parameters_struct_type {
	hex_generator_display_struct_type generator_display_data;

	std::string  expression_file_pathname = "";
	std::string  expression_file_name     = "...";

	float	 x_start = -1.0f, x_end = 1.0f;
	float	 y_start = -1.0f, y_end = 1.0f;

	float	 resolution_step = 0.01f;
	//float	 generation_threshold = 0.1f;
	float    e_time = 0.0f;// , c_time = 0.0f;
	float    frame = 0.0f;

	int      invocation = 256;// this value may need to be changed to a maximum value

	int  	max_surface_value = 255, min_surface_value = 1; // *****

	std::vector<hex_surface_generator_parameter_variable_struct_type>      variables;
	std::vector<hex_surface_generator_parameter_int_variable_struct_type>  int_variables;
	std::vector<hex_surface_generator_parameter_bool_variable_struct_type> bool_variables;


	int get_variable_index(std::string variable_name) {
		int i = -1;
		for(hex_surface_generator_parameter_variable_struct_type variable : variables) {
			i++;
			if (variables[i].variable_name == variable_name) {
				return i;
			}
		}
		return -1;
	}

	bool change_variable_value(std::string variable_name, float value) {
		// Do not use a foreach loop to change variable values as the changes are only local and will not 
		// carry over outside the loop unless changes are made to pointers or pointer content
		for (int i = 0; i < variables.size(); i++) {
			if (variables[i].variable_name == variable_name) {
				variables[i].value = value;
//QMessageBox::information(0, "Function Expression Success", variables[i].variable_name + "  changed : "+std::string::number(value)+":"+std::string::number(variables[i].variable), QMessageBox::Ok); // testing only;
				return true;
			}
		}

		return false;
	}

	float get_variable_value(std::string variable_name) {
		for (hex_surface_generator_parameter_variable_struct_type variable : variables) {
			if (variable.variable_name == variable_name) {
				return variable.value;
			}
		}
		return NAN;
	}

};