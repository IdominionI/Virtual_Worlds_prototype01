#pragma once

#include <Source/Graphics_Engine/Compute/compute_shader.h>
//#include <Source/Graphics_Engine/Compute/import_compute_expression_code.h>

#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include "../Hex_surface_object/DataTypes/dt_hex_surface_generator.h"
#include "../Hex_surface_object/hex_surface_object.h"


/*
	hex_surface_compute_generator_class

	C++ class that generates by use of a glsl compute shader program the hex suface
	data and stores the results in the hex suface data matrix.

	The OpenGL glsl compute shader program is created from several source text files.

	The source code is split into specific text files for different sections of the glsl program
	so as to create the ability for the application to dynamicly allow the glsl program to accomodate
	interactive definitions and assignments of glsl shader variables. This was also done for the user
	to write custom code targeted to be used in the glsl programs main function.

	This class imports all the source code text files of each of these sections and them combines them
	into a single Qt std::string class storage variable ready for adding to a OpenGL shader program for
	compilation
*/

class hex_surface_compute_generator_class : public compute_shader_class {
public:
	hex_surface_object_class *cloud = NULL; // pointer to the hex suface class in the virtual worlds scene data model
	log_panel_class          *log_panel = NULL;

	bool generate_hex_surface_function();
	bool update_hex_surface_generation();
	void create_compute_shader_source_code();
	bool hex_surface_generation_execute(hex_surface_data_type* buffer, int size, int local_x_group_work_size);
	bool create_hex_surface_matrix(hex_surface_object_class* cloud, hex_surface_generator_parameters_struct_type  hex_surface_generator_parameters);

private:
	shader_class shader;

	hex_surface_generator_parameters_struct_type hex_surface_generator_parameters;

	std::vector<hex_surface_generator_function_variable_struct_type> function_variables;

	std::string version_s, work_group_invocations_s, reserved_uniforms_s, functions_s, main_s, output_s;
	std::string user_uniforms_s, user_functions_s, user_main_s;

	void define_compute_version();
	void define_work_group_invocations(int invocations);
	void define_reserved_uniforms();
	void define_user_uniforms();
	void define_function_s();
	void define_main();
	void define_ouput();

};
