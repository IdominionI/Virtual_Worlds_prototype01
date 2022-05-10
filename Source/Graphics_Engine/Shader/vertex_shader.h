#pragma once

#include "vw_shader_basis.h"

/*
	vw_vertex_shader_class

	C++ class to create code to define an OpenGL glsl vertex shader program from source text files.

	The source code is split into specific text files for different sections of the glsl program
	so as to create the ability for the application to dynamicly allow the glsl program to accomodate
	interactive definitions and assignments of glsl shader variables. This was also done for the user
	to write custom code targeted to be used in the glsl programs main function.

	This class imports all the source code text files of each of these sections and them combines them
	into a single Qt QString class storage variable ready for adding to a OpenGL shader program for
	compilation
*/

class vw_vertex_shader_class : public vw_shader_basis_class {
public:

	bool generate_vertex_shader_code() {
		import_compute_expression_class import_compute_expression;  // Class to import cutom user glsl code

		import_compute_expression.filename_to_read = shader_parameters.vertex_shader_file_pathname; // Define file pathname of custom user glsl code file to import
//printf("generate_vertex_shader_code 0000 %s :: %s \n", import_compute_expression.filename_to_read.c_str(), shader_parameters.vertex_shader_file_pathname.c_str());
		if (!import_compute_expression.import_compute_expression()) { // Import Custom user glsl code
//printf("generate_vertex_shader_code 1111\n");
			//QMessageBox::information(0, "Function Expression Success", "vw_vertex_shader_class : generate_vertex_shader_code: import user vertex shader code of file " + shader_parameters.vertex_shader_file_pathname + " failed.", QMessageBox::Ok); // testing only
			printf("vw_vertex_shader_class : generate_vertex_shader_code: import user vertex shader code of file %s failed.\n", shader_parameters.vertex_shader_file_pathname.c_str());
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

		create_vertex_shader_source_code(); // Colate all OpenGL glsl code sections into one glgl source code program
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
	void create_vertex_shader_source_code() {
		shader_code = "";
		shader_code += version_s;
		//shader_code += "#version 450 core\n";
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
		shader_code += "// ++++++++++++++ User defined voxel output function ++++++++++++++\n";
		shader_code += "\n";
		shader_code += user_main_s;
		shader_code += "\n";
		shader_code += "// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
		shader_code += output_s;

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, " voxel_compute_generator_class :: create_compute_shader_source_code" + shader_code);
//}
	}

	// +++++++++++ User dynamicly defined uniforms ++++++++++++++
	// Dynamicly update the uniform vertex variables of the OpenGL shader program
	//bool update_shader_variables(QOpenGLShaderProgram *ogl_shader_program) {
/*	bool update_shader_variables() {
		//if (ogl_shader_program == NULL) {
			//if (log_widget != NULL) {
			//	log_widget->log_message(log_display, log_message_type_enum_type::critical, "vw_vertex_shader_class: update_shader_variables :: Could not update the vertex shader user variables :- undefined shader program.");
			//}
		//	printf("vw_vertex_shader_class: update_shader_variables :: Could not update the vertex shader user variables :- undefined shader program.");
		//	return false;
		//}

		for (shader_parameter_variable_struct_type variable : shader_parameters.variables) {
			ogl_shader_program->setUniformValue(variable.variable_name.toStdString().c_str(), variable.value);
		}

		for(shader_parameter_int_variable_struct_type int_variable : shader_parameters.int_variables) {
			ogl_shader_program->setUniformValue(int_variable.variable_name.toStdString().c_str(), int_variable.value);
		}

		for (shader_parameter_bool_variable_struct_type bool_variable : shader_parameters.bool_variables) {
			ogl_shader_program->setUniformValue(bool_variable.variable_name.toStdString().c_str(), bool_variable.value);
		}
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		return true;
	}
*/

private:
	void define_reserved_uniforms() {
		reserved_uniforms_s = import_code(shader_basis_code_directory_pathname+"vs_reserved_uniforms.txt");
		/*
		reserved_uniforms_s =  "uniform mat4 mvpMatrix; //model-view-projection matrix\n";
		reserved_uniforms_s += "in vec4 vertex;         //vertex that the shader is processing\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "in float voxel_value; // Value assigned to Vertex\n";
		reserved_uniforms_s += "in float voxel_color; // Color assigned to Vertex\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "// Voxel parameter data\n";
		reserved_uniforms_s += "uniform ivec3 voxel_matrix_dimension;\n";
		reserved_uniforms_s += "uniform vec3  voxel_origin;\n";
		reserved_uniforms_s += "uniform float voxel_hcp_z_increment;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "in float voxel_size;\n";
		reserved_uniforms_s += "flat out float voxSize; \n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "//voxel surface display data\n";
		reserved_uniforms_s += "uniform float voxel_min_surface_display_value;\n";
		reserved_uniforms_s += "uniform float voxel_max_surface_display_value;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "// Camera data\n";
		reserved_uniforms_s += "uniform vec3  camera_loc;         // Camera location\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "uniform bool  use_camera_lighting;\n";
		reserved_uniforms_s += "uniform vec3  lighting_camera_offset;\n";
		reserved_uniforms_s += "uniform int   camera_lighting_type;\n";
		reserved_uniforms_s += "uniform float lighting_intensity;\n";
		reserved_uniforms_s += "uniform vec3  camera_front_vector;\n";
		reserved_uniforms_s += "uniform vec3  camera_up_vector;\n";
		reserved_uniforms_s += "uniform vec3  camera_right_vector;\n";
		reserved_uniforms_s += "// Cannot use same variable name for in and out variables \n";
		reserved_uniforms_s += "// use Input and output interface blocks to solve this\n";
		reserved_uniforms_s += "// The following output block is to allow  varyingColor to be passed to either \n";
		reserved_uniforms_s += "// a geometry shader to be psssed on to the fragment shader, or to the fragemt alone\n";
		reserved_uniforms_s += "out VertexData\n";
		reserved_uniforms_s += "{\n";
		reserved_uniforms_s += "float  value;\n";
		reserved_uniforms_s += " vec4 varyingColor;\n";
		reserved_uniforms_s += "} vs_out;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "vec4 raw_color;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "#define TRUE  0\n";
		reserved_uniforms_s += "#define FALSE 1\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "uniform int   t_frame;\n";
		reserved_uniforms_s += "uniform float t_e_time;\n";
		reserved_uniforms_s += "uniform float t_c_time;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "uniform vec2  origin;\n";
		reserved_uniforms_s += "uniform vec2  resolution;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "in int vEnabledFaces;\n";
		reserved_uniforms_s += "flat out int gEnabledFaces;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "// Lighting and light shading\n";
		reserved_uniforms_s += "uniform bool  display_vertex_light_shading;\n";
		reserved_uniforms_s += "uniform vec3  lighting_direction;\n";
		reserved_uniforms_s += "uniform vec3  ambient_light;\n";
		reserved_uniforms_s += "uniform float specular_strength;\n";
		reserved_uniforms_s += "uniform vec3  light_color;\n";
		reserved_uniforms_s += "//color of vertex that shader is processing\n";
		reserved_uniforms_s += "uniform vec4  color;\n";
		reserved_uniforms_s += "uniform int  display_native_vertex_colors;\n";
		*/
	}
	
	void  define_user_uniforms() {
		user_uniforms_s = "";

		for (shader_parameter_variable_struct_type variable : shader_parameters.variables) {
			if(variable.active_variable)
				user_uniforms_s += "uniform float " + variable.variable_name + ";\n";
		}

		for (shader_parameter_int_variable_struct_type int_variable : shader_parameters.int_variables) {
			if (int_variable.active_variable)
				user_uniforms_s += "uniform int " + int_variable.variable_name + ";\n";
		}

		for(shader_parameter_bool_variable_struct_type bool_variable : shader_parameters.bool_variables) {
			if (bool_variable.active_variable)
				user_uniforms_s += "uniform int " + bool_variable.variable_name + ";\n";
		}

	}

	void define_function_s() {
		functions_s = import_code(shader_basis_code_directory_pathname + "vs_defined_functions.txt");
		/*
		functions_s = "// ******************* following only works for RGB values *********************\n";
		functions_s += "// *********** Obtained method from question and answer posted on stackoverflow but only is valid for first three RGB color values *************\n";
		functions_s += "const vec4 bitEnc = vec4(1., 255., 65025., 16581375.);\n";
		functions_s += "const vec4 bitDec = 1. / bitEnc;\n";
		functions_s += "vec4 encode_float_to_RGB(float v) {\n";
		functions_s += "	vec4 enc = bitEnc * v;\n";
		functions_s += "	enc = fract(enc);\n";
		functions_s += "	enc -= enc.yzww * vec2(1. / 255., 0.).xxxy;\n";
		functions_s += "	return enc;\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "//float DecodeFloatRGBA (vec4 v) {\n";
		functions_s += "//    return dot(v, bitDec);\n";
		functions_s += "//}\n";
		functions_s += "// ******************************************\n";
		*/
		
	}

	void define_main() {
		main_s = import_code(shader_basis_code_directory_pathname + "vs_main.txt");
/*
		main_s = "void main() {\n";
		main_s += "	voxSize = voxel_size;\n";
		main_s += "\n";
		main_s += "vec4 rgb = encode_float_to_RGB(voxel_color)/255.0;\n";
		main_s += "rgb.a = 1.0;\n";
		main_s += "raw_color = rgb;\n";
		main_s += "float value = voxel_value;\n";
*/
	}

	void define_ouput() {
		output_s = import_code(shader_basis_code_directory_pathname + "vs_output.txt");
/*
		output_s = "vs_out.varyingColor = raw_color;\n";
		output_s += "	   vs_out.value = value;\n";
		output_s += "\n";
		output_s += "gEnabledFaces = vEnabledFaces;\n";
		output_s += "}\n";
*/
	}

};