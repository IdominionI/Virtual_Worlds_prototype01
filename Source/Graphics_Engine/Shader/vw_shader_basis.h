#pragma once

//#include <VW_Graphics/Compute/import_compute_expression_code.h>
#include "../Compute/import_compute_expression_code.h"
#include "import_shader_code.h"

#include "shader_parameters.h"

enum class ogl_shader_version_enum { v41, v43, v45 };// OpenGL version that the shader code is compiled for

/*
	vw_shader_basis_class

	C++ basis class that has functions, data types and variables that are of common use to multiple
	and similar shader classes that inherit this class. 

	This basis Class enables derived shader classes to import and store OpenGL glsl shader code ready for
	complilation.

*/

class vw_shader_basis_class : public import_shader_code_class {
public:
	//log_widget_class       *log_widget = NULL;// pointer to log widget to report to 
	//log_display_struct_type log_display;

	shader_parameters_struct_type shader_parameters; // structure variable to store the user defined variables to add to the shader program code and update values
	//bool    shader_defined = false;                  // variable to define if shader code is ready to be used

	std::string                 shader_basis_code_directory_pathname = "";
	std::string                 shader_code = "";							// QString class to store shader code
	ogl_shader_version_enum ogl_version = ogl_shader_version_enum::v45; // default OpenGl version that can be changed

	//vw_shader_basis_class(log_widget_class *log_widget_ = NULL) {
	vw_shader_basis_class() {
		//log_widget = log_widget_;
	}
	std::string version_s;
protected :
	// Common variable names that is to be used in all deried classes to store in QString classes the
	// segments of glsl code wich are to be combined into one single glsl QString class to define a
	// complete glsl shader program
	std::string  work_group_invocations_s, reserved_uniforms_s, functions_s, main_s, output_s; //
	std::string user_uniforms_s, user_functions_s, user_main_s;  // User defined uniforms, functions and main glsl code sections

	// Define the OpenGL version that the glsl is targeted to be used with as a QString class text variable version_s
	void define_compute_version() {

		if (ogl_version == ogl_shader_version_enum::v41)
			version_s = "#version 410 core\n";
		else
		if (ogl_version == ogl_shader_version_enum::v45)
			version_s = "#version 450 core\n";
		else
			version_s = "#version 430 core\n";

	}



};