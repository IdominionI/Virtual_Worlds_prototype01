#pragma once
/*
 NOTE ::
 This is a modified vesion of the Cinder Project shader format class but retains much of the same functionality
 and routines that exist in the original Cinder code
 */


#include <string>
#include <vector>
#include <array>
//#include <typeinfo>

#include <filesystem>

#include <Universal/3rdparty/include/gl/glew.h>
#include <Universal/3rdparty/include/glm/glm.hpp>
#include <Universal/Framework/FWfile_tools.h>

#include "shader_parameters.h"
//#include "shader_manager.h" // cannot include

enum class shader_type_enum{vertex,geometry,fragment,tesselation,compute};
enum class shader_source_type_enum{pathname,code};

struct uniform_struct_type {
	std::string                                   name      = "";
	application_default_shader_variable_type_enum data_type = application_default_shader_variable_type_enum::none;
};


struct shader_format_class {
	shader_format_class() {
		initialise();
	}

	GLuint	    shader_program_id = 0;				   // ID number of shader program witrh the
	idu_type    entity_category_id = NULL_CATEGORY_ID; // Data type that the shader can be applied to
	string_type shader_name = "";				       // The name of the shader program
	string_type shader_description = "";			   // A descripriton of the shader program
	
	bool vertex_pathname_defined     = false;
	bool geometry_pathname_defined   = false;
	bool fragment_pathname_defined   = false;
	bool tesslation_pathname_defined = false;
	bool compute_pathname_defined    = false;

	bool vertex_source_code_defined     = false;
	bool geometry_source_code_defined   = false;
	bool fragment_source_code_defined   = false;
	bool tesslation_source_code_defined = false;
	bool compute_source_code_defined    = false;

	std::string vertex_shader_source_code        = "";
	std::filesystem::path glsl_vertex_shader_file_pathname = "";

	std::string point_shader_source_code = "";
	std::filesystem::path glsl_point_shader_file_pathname = "";

	std::string geometry_shader_source_code        = "";
	std::filesystem::path glsl_geometry_shader_file_pathname = "";

	std::string fragment_shader_source_code        = "";
	std::filesystem::path glsl_fragment_shader_file_pathname = "";

	//std::string tesselation_shader_source_code        = "";
	//std::filesystem::path glsl_tesselation_shader_file_pathname = "";

	std::string compute_shader_source_code        = "";
	std::filesystem::path compute_shader_file_pathname = "";

	std::filesystem::path vertex_shader_file_pathname   = "";
	std::filesystem::path geometry_shader_file_pathname = "";
	std::filesystem::path point_shader_file_pathname    = "";
	std::filesystem::path fragment_shader_file_pathname = "";
	//???? are the following needed ???????
	std::filesystem::path default_vertex_shader_file_pathname   = "";
	std::filesystem::path default_geometry_shader_file_pathname = "";
	std::filesystem::path default_point_shader_file_pathname    = "";
	std::filesystem::path default_fragment_shader_file_pathname = "";

	bool use_default_vertex_shader   = true;
	bool use_default_geometry_shader = true;
	bool use_default_point_shader    = true;
	bool use_default_fragment_shader = true;

	bool use_point_shader = true;
	bool animate_shaders  = false;

	int frame;

	std::vector<uniform_struct_type> user_uniforms;

	void initialise() {
		shader_program_id  = 0;
		entity_category_id = NULL_CATEGORY_ID;

		vertex_pathname_defined     = false;
		geometry_pathname_defined   = false;
		fragment_pathname_defined   = false;
		tesslation_pathname_defined = false;
		compute_pathname_defined    = false;

		vertex_source_code_defined     = false;
		geometry_source_code_defined   = false;
		fragment_source_code_defined   = false;
		tesslation_source_code_defined = false;
		compute_source_code_defined    = false;


		vertex_shader_file_pathname.clear();
		default_vertex_shader_file_pathname.clear();
		
		point_shader_file_pathname.clear();
		default_point_shader_file_pathname.clear();

		geometry_shader_file_pathname.clear();
		default_geometry_shader_file_pathname.clear();
		
		fragment_shader_file_pathname.clear();
		default_fragment_shader_file_pathname.clear();
		

		vertex_shader_source_code.clear();
		geometry_shader_source_code.clear();
		fragment_shader_source_code.clear();

		glsl_vertex_shader_file_pathname.clear();
		glsl_geometry_shader_file_pathname.clear();
		glsl_fragment_shader_file_pathname.clear();
		
		//tesselation_shader_source_code.clear();
		//glsl_tesselation_shader_file_pathname.clear();
		
		compute_shader_source_code.clear();
		compute_shader_file_pathname.clear();

		//clear_uniforms();
		//mandatory_uniforms.clear();
		user_uniforms.clear();
	}
	

	bool define_shader_to_compile(shader_type_enum shader_type, std::string shader_source_pathname) {
		std::string full_pathname = "";

		define_shader_source_pathname(shader_type, shader_source_pathname);
		
		switch (shader_type) {
			case shader_type_enum::vertex      : full_pathname = std::filesystem::absolute(glsl_vertex_shader_file_pathname).string();      break;
			case shader_type_enum::geometry    : full_pathname = std::filesystem::absolute(glsl_geometry_shader_file_pathname).string();    break;
			case shader_type_enum::fragment    : full_pathname = std::filesystem::absolute(glsl_fragment_shader_file_pathname).string();    break;
			//case shader_type_enum::tesselation : full_pathname = std::filesystem::absolute(glsl_tesselation_shader_file_pathname).string(); break;
			case shader_type_enum::compute     : full_pathname = std::filesystem::absolute(compute_shader_file_pathname).string();          break;
		}

//printf("define_shader_to_compile 000 %s : %s \n", shader_source_pathname.c_str(), full_pathname.c_str());

		std::string source_code = FW::filetools::read_all(full_pathname);
//printf("define_shader_to_compile 111 %s : \n %s \n", full_pathname.c_str(), source_code.c_str());
		if (source_code.size() < 1) return false;

		define_shader_source_code(shader_type, source_code);

		return true;
	}

	shader_format_class& define_shader(shader_type_enum shader_type, shader_source_type_enum shader_source_type, std::string shader_source){
		switch (shader_source_type) {
			case shader_source_type_enum::pathname : return define_shader_source_pathname(shader_type, shader_source); break;
			case shader_source_type_enum::code     : return define_shader_source_code(shader_type, shader_source);     break;
		}

		return *this;
	}

	shader_format_class& define_shader_source_code(shader_type_enum shader_type, std::string shader_code) {
		switch (shader_type) {
			case shader_type_enum::vertex      : vertex_shader_source_code      = shader_code; vertex_source_code_defined     = true; break;
			case shader_type_enum::geometry    : geometry_shader_source_code    = shader_code; geometry_source_code_defined   = true; break;
			case shader_type_enum::fragment    : fragment_shader_source_code    = shader_code; fragment_source_code_defined   = true; break;
			//case shader_type_enum::tesselation : tesselation_shader_source_code = shader_code; tesslation_source_code_defined = true; break;
			case shader_type_enum::compute     : compute_shader_source_code     = shader_code; compute_source_code_defined    = true; break;
		}

		return *this;
	}

	shader_format_class& define_shader_source_pathname(shader_type_enum shader_type, std::string shader_source_pathname) {
		switch (shader_type) {
			case shader_type_enum::vertex      : glsl_vertex_shader_file_pathname      = shader_source_pathname; vertex_pathname_defined     = true; break;
			case shader_type_enum::geometry    : glsl_geometry_shader_file_pathname    = shader_source_pathname; geometry_pathname_defined   = true; break;
			case shader_type_enum::fragment    : glsl_fragment_shader_file_pathname    = shader_source_pathname; fragment_pathname_defined   = true; break;
			//case shader_type_enum::tesselation : tesselation_shader_file_pathname = shader_source_pathname; tesslation_pathname_defined = true; break;
			case shader_type_enum::compute     : compute_shader_file_pathname     = shader_source_pathname; compute_pathname_defined    = true; break;
		}

		return *this;
	}

	// ##################################
	/*
	// Decided not to pursue with generating universal uniforms to be automatically
	// injected into a glsl program Left here in case needed in future
	// Commented out uniforms to be added when found essential to do so
	void define_madatory_uniforms() {
		shader_mandatory_uniform_struct_type su;

		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_MODEL_MATRIX;                   su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_MODEL_MATRIX_INVERSE;           su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_MODEL_MATRIX_INVERSE_TRANSPOSE; su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		su.name = "view"; su.type=mandatory_uniform_type_enum::UNIFORM_VIEW_MATRIX;                  su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_VIEW_MATRIX_INVERSE;            su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_MODEL_VIEW;                     su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_MODEL_VIEW_INVERSE;             su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_MODEL_VIEW_INVERSE_TRANSPOSE;   su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		su.name = "mvpMatrix"; su.type=mandatory_uniform_type_enum::UNIFORM_MODEL_VIEW_PROJECTION;   su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_MODEL_VIEW_PROJECTION_INVERSE;  su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		su.name = "projection"; su.type=mandatory_uniform_type_enum::UNIFORM_PROJECTION_MATRIX;      su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_PROJECTION_MATRIX_INVERSE;      su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_VIEW_PROJECTION;                su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_NORMAL_MATRIX;                  su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_VIEWPORT_MATRIX;                su.GL_type = GL_FLOAT_MAT4; mandatory_uniforms.push_back(su);
		//su.name = ""; su.type=mandatory_uniform_type_enum::UNIFORM_WINDOW_SIZE;                    su.GL_type = GL_INT_VEC2;   mandatory_uniforms.push_back(su);
		//su.name = "t_time"; su.type=mandatory_uniform_type_enum::UNIFORM_ELAPSED_SECONDS;            su.GL_type = GL_FLOAT;      mandatory_uniforms.push_back(su);

		su.name = "camera_loc";          su.type = mandatory_uniform_type_enum::UNIFORM_CAMERA_LOCATION;     su.GL_type = GL_INT_VEC3; mandatory_uniforms.push_back(su);
		su.name = "camera_front_vector"; su.type = mandatory_uniform_type_enum::UNIFORM_CAMERA_FRONT_VECTOR; su.GL_type = GL_INT_VEC3; mandatory_uniforms.push_back(su);
		su.name = "camera_up_vector";    su.type = mandatory_uniform_type_enum::UNIFORM_CAMERA_UP_VECTOR;    su.GL_type = GL_INT_VEC3; mandatory_uniforms.push_back(su);
		su.name = "camera_right_vector"; su.type = mandatory_uniform_type_enum::UNIFORM_CAMERA_RIGHT_VECTOR; su.GL_type = GL_INT_VEC3; mandatory_uniforms.push_back(su);
			
		su.name = "ambience";           su.type=mandatory_uniform_type_enum::UNIFORM_WORLD_AMBIENCE;           su.GL_type = GL_FLOAT;    mandatory_uniforms.push_back(su);
		su.name = "specular_strength";  su.type=mandatory_uniform_type_enum::UNIFORM_WORLD_SPECULAR_STRENGTH;  su.GL_type = GL_FLOAT;    mandatory_uniforms.push_back(su);
		su.name = "lighting_intensity"; su.type=mandatory_uniform_type_enum::UNIFORM_WORLD_LIGHTING_INTENSITY; su.GL_type = GL_FLOAT;    mandatory_uniforms.push_back(su);
		su.name = "lighting_direction"; su.type=mandatory_uniform_type_enum::UNIFORM_WORLD_LIGHTING_DIRECTION; su.GL_type = GL_INT_VEC3; mandatory_uniforms.push_back(su);
		su.name = "light_color";        su.type=mandatory_uniform_type_enum::UNIFORM_WORLD_LIGHT_COLOR;        su.GL_type = GL_INT_VEC4; mandatory_uniforms.push_back(su);
		
		su.name = "camera_lighting_type";   su.type=mandatory_uniform_type_enum::UNIFORM_WORLD_CAMERA_LIGHTING_TYPE;   su.GL_type = GL_INT;      mandatory_uniforms.push_back(su);
		su.name = "use_camera_lighting";    su.type=mandatory_uniform_type_enum::UNIFORM_WORLD_USE_CAMERA_LIGHTING;    su.GL_type = GL_INT;      mandatory_uniforms.push_back(su);
		su.name = "lighting_camera_offset"; su.type=mandatory_uniform_type_enum::UNIFORM_WORLD_LIGHTING_CAMERA_OFFSET; su.GL_type = GL_INT_VEC3; mandatory_uniforms.push_back(su);
		
		su.name = "t_frame"; su.type = mandatory_uniform_type_enum::UNIFORM_FRAME; su.GL_type = GL_INT;        mandatory_uniforms.push_back(su);
	}
*/

	//shader_format_class& add_uniform(std::string uniform_name, user_uniform_type_enum uniform_type= user_uniform_type_enum::UNIFORM_USER_DEFINED, 
	//								 application_default_shader_variable_type_enum glsl_type = application_default_shader_variable_type_enum::Float1) {
	shader_format_class& add_uniform(std::string uniform_name, application_default_shader_variable_type_enum glsl_type = application_default_shader_variable_type_enum::Float1) {
		

		if (uniform_exists(uniform_name)) {
			printf("ERROR : Could not add uniform of name %s to shader format to create glsl shader program. A uniform of the same name is currently defined.\n", uniform_name.c_str());
			return *this;
		}

		uniform_struct_type su;
		su.name      = uniform_name;
		su.data_type = glsl_type;
		//su.glsl_type = GL_type;

		user_uniforms.push_back(su);

		return *this;
	}

	void clear_uniforms() {
		user_uniforms.clear();
		user_uniforms.shrink_to_fit();
	}

	bool uniform_exists(std::string uniform_name) {
		for (uniform_struct_type uniform : user_uniforms) {
			if (uniform.name == uniform_name) {
				return true;
				break;
			}
		}

		return false;
	}

	bool delete_uniform(std::string uniform_name) {
		for (int i = 0; i < user_uniforms.size();i++) {
			if (user_uniforms[i].name == uniform_name) {
				return delete_uniform(i);
				break;
			}
		}

		return false;
	}

	bool delete_uniform(int index) {
		if (index<0 || index >= user_uniforms.size()) return false;
		user_uniforms.erase(user_uniforms.begin() + index);
		return true;
	}

};