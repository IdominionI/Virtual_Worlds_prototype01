#pragma once

#include "vw_shader_basis.h"
/*
	vw_geometry_shader_class

	C++ class to create code to define an OpenGL glsl geometry shader program from source text files.

	The source code is split into specific text files for different sections of the glsl program
	so as to create the ability for the application to dynamicly allow the glsl program to accomodate
	interactive definitions and assignments of glsl shader variables. This was also done for the user
	to write custom code targeted to be used in the glsl programs main function.

	This class imports all the source code text files of each of these sections and them combines them
	into a single Qt QString class storage variable ready for adding to a OpenGL shader program for
	compilation
*/
class vw_geometry_shader_class : public vw_shader_basis_class {
public:
	bool generate_geometry_shader_code() {
		import_compute_expression_class import_compute_expression;// Class to import cutom user glsl code

		import_compute_expression.filename_to_read = shader_parameters.geometry_shader_file_pathname;// Define file pathname of custom user glsl code file to import

		if (!import_compute_expression.import_compute_expression()) {
			//QMessageBox::information(0, "Function Expression Success", "vw_geometry_shader_class : generate_geometry_shader_code: import user voxel geometry shader code of file " + shader_parameters.geometry_shader_file_pathname + " failed.", QMessageBox::Ok); // testing only
			printf("vw_geometry_shader_class : generate_geometry_shader_code: import user voxel geometry shader code of file %s failed.", shader_parameters.geometry_shader_file_pathname.c_str());
			return false;
		}

		// Import the different OpenGL Shader code sections into their respective Qt QString class variables
		define_compute_version();

		define_reserved_uniforms();

		define_user_uniforms();

		define_function_s();

		user_functions_s = import_compute_expression.function_code;
		define_main();

		user_main_s = import_compute_expression.expression_code;
		define_ouput();

		create_geometry_shader_source_code(); // Colate all OpenGL glsl code sections into one glgl source code program
//QMessageBox::information(0, "Function Expression Success", "vw_vertex_shader_class : generate_vertex_shader_code() 005", QMessageBox::Ok); // testing only
/*
		if (!define_compute_program()) {
			if (log_widget != NULL) {
				log_widget->log_message(log_display, log_message_type_enum_type::critical, "vertex shader generation: generate vertex shader program :: Could not create the vertex  shader program.");
			}
			return false;
		}
*/
//QMessageBox::information(0, "Function Expression Success", "vw_vertex_shader_class : generate_vertex_shader_code() 006", QMessageBox::Ok); // testing only
		return true;
	}

	// Collate all OpenGL glsl code sections into one glgl source code program
	void create_geometry_shader_source_code() {
		shader_code = "";
		shader_code += version_s;
		//shader_code += "#version 450 core\n";
		shader_code += "\n";
		shader_code += "// ###### Shader inputs - ouputs #######\n";
		shader_code += reserved_uniforms_s;
		shader_code += "// ####################################################\n";
		shader_code += "\n";
		shader_code += "// +++++++++++++ Dynamicaly created user variables +++++++++++++++\n";
		shader_code += "\n";
		shader_code += user_uniforms_s;
		shader_code += "\n";
		shader_code += "// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
		shader_code += "\n";
		shader_code += functions_s;
		shader_code += "\n";
		shader_code += "// ******** User defined functions to be added here ********\n";
		shader_code += "\n";
		shader_code += user_functions_s;
		shader_code += "\n";
		shader_code += "// *********************************************************\n";
		shader_code += "\n";
		shader_code += main_s;
		shader_code += "\n";
		shader_code += "// ++++++++++++++ User defined output function ++++++++++++++\n";
		shader_code += "\n";
		shader_code += user_main_s;
		shader_code += "\n";
		shader_code += "// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
		shader_code += output_s;

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, " voxel_compute_generator_class :: create_compute_shader_source_code" + shader_code);
//}
	}
/*
	// +++++++++++ User dynamicly defined uniforms ++++++++++++++
	// Dynamicly update the uniform vertex variables of the OpenGL shader program
	bool update_shader_variables(QOpenGLShaderProgram *ogl_shader_program) {

		if (ogl_shader_program == NULL) {
			if (log_widget != NULL) {
				log_widget->log_message(log_display, log_message_type_enum_type::critical, "vw_vertex_shader_class: update_shader_variables :: Could not update the vertex shader user variables :- undefined shader program.");
			}
			return false;
		}

		foreach(shader_parameter_variable_struct_type variable, shader_parameters.variables) {
			//if(variable.pass_to_geometry_shader)
			if(variable.active_variable && variable.pass_to_geometry_shader)
				ogl_shader_program->setUniformValue(variable.variable_name.toStdString().c_str(), variable.value);
		}

		foreach(shader_parameter_int_variable_struct_type int_variable, shader_parameters.int_variables) {
			//if (int_variable.pass_to_geometry_shader)
			if (int_variable.active_variable && int_variable.pass_to_geometry_shader)
				ogl_shader_program->setUniformValue(int_variable.variable_name.toStdString().c_str(), int_variable.value);
		}

		foreach(shader_parameter_bool_variable_struct_type bool_variable, shader_parameters.bool_variables) {
			//if (bool_variable.pass_to_geometry_shader)
			if (bool_variable.active_variable && bool_variable.pass_to_geometry_shader)
				ogl_shader_program->setUniformValue(bool_variable.variable_name.toStdString().c_str(), bool_variable.value);
		}

		return true;
	}
	*/

private:
	void define_compute_version() {

		if (ogl_version == ogl_shader_version_enum::v41)
			version_s = "#version 410\n";
		else
		if (ogl_version == ogl_shader_version_enum::v45)
			version_s = "#version 450\n";
		else
			version_s = "#version 430\n";
	}

	void define_reserved_uniforms() {
		reserved_uniforms_s = import_code(shader_basis_code_directory_pathname + "gs_reserved_uniforms.txt");
/*
		reserved_uniforms_s =  "layout(points) in;\n";
		reserved_uniforms_s += "layout(triangle_strip, max_vertices = 48) out;\n";
		reserved_uniforms_s += "// for points need the following\n";
		reserved_uniforms_s += "//layout(points, max_vertices = 1) out;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "flat in int gEnabledFaces[];// Must have as flat array otherwise will not work\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "flat in float voxSize[];\n";
		reserved_uniforms_s += "uniform mat4 mvpMatrix; // ************model-view-projection matrix \n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "//voxel surface display data\n";
		reserved_uniforms_s += "uniform float voxel_min_surface_display_value;\n";
		reserved_uniforms_s += "uniform float voxel_max_surface_display_value;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "uniform vec3 camera_loc;   // Camera location\n";
		reserved_uniforms_s += "uniform bool use_camera_lighting;\n";
		reserved_uniforms_s += "uniform vec3 lighting_camera_offset;\n";
		reserved_uniforms_s += "uniform int  camera_lighting_type;\n";
		reserved_uniforms_s += "uniform float lighting_intensity;\n";
		reserved_uniforms_s += "uniform vec3 camera_front_vector;\n";
		reserved_uniforms_s += "uniform vec3 camera_up_vector;\n";
		reserved_uniforms_s += "uniform vec3 camera_right_vector;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "uniform vec3 voxel_origin;\n";
		reserved_uniforms_s += "uniform float voxel_hcp_z_increment;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "// Cannot use same variable name for in and out variables \n";
		reserved_uniforms_s += "// use Input and output interface blocks to solve this\n";
		reserved_uniforms_s += "// The following input/output block is to allow  varyingColor \n";
		reserved_uniforms_s += "// to be passed to the fragment shader or without changing the name\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "in VertexData\n";
		reserved_uniforms_s += "{\n";
		reserved_uniforms_s += " float value;\n";
		reserved_uniforms_s += " vec4  varyingColor;\n";
		reserved_uniforms_s += "} gs_in[]; // Must have as an array otherwise will not work as one for each input vertex,\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "out VertexData\n";
		reserved_uniforms_s += "{\n";
		reserved_uniforms_s += "  vec4 varyingColor;\n";
		reserved_uniforms_s += "} gs_out;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "// Lighting and light shading\n";
		reserved_uniforms_s += "uniform bool  display_vertex_light_shading;\n";
		reserved_uniforms_s += "uniform vec3  lighting_direction;\n";
		reserved_uniforms_s += "uniform vec3  ambient_light;\n";
		reserved_uniforms_s += "uniform float specular_strength;\n";
		reserved_uniforms_s += "uniform vec3  light_color;\n";
		reserved_uniforms_s += "\n";
*/
	}
	
	void  define_user_uniforms() {
		user_uniforms_s = "";

		for (shader_parameter_variable_struct_type variable : shader_parameters.variables) {
			//if (variable.pass_to_geometry_shader)
			if (variable.active_variable)
				user_uniforms_s += "uniform float " + variable.variable_name + ";\n";
		}

		for (shader_parameter_int_variable_struct_type int_variable : shader_parameters.int_variables) {
			//if (int_variable.pass_to_geometry_shader)
			if (int_variable.active_variable)
				user_uniforms_s += "uniform int " + int_variable.variable_name + ";\n";
		}

		for (shader_parameter_bool_variable_struct_type bool_variable : shader_parameters.bool_variables) {
			//if (bool_variable.pass_to_geometry_shader)
			if (bool_variable.active_variable)
				user_uniforms_s += "uniform int " + bool_variable.variable_name + ";\n";
		}

	}

	void define_function_s() {
		functions_s = import_code(shader_basis_code_directory_pathname + "gs_defined_functions.txt");
		//functions_s = "// ******************* No in Built functions defined at this time *********************\n";
	}


	void define_main() {
		main_s = import_code(shader_basis_code_directory_pathname + "gs_main.txt");
		//main_s = "void main() {\n";
	}

	void define_ouput() {
		output_s = import_code(shader_basis_code_directory_pathname + "gs_output.txt");
		//output_s = "}\n";
	}

};