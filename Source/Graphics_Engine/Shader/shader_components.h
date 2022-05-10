#pragma once

//#include <Universal/3rdparty/include/glm/glm.hpp>

#include "shader_parameters.h"
#include "shader_manager.h"

struct shader_component_struct_type : shader_parameters_struct_type {
	GLuint	    shader_program_id = 0;				   // ID number of shader program witrh the
	idu_type    entity_category_id = NULL_CATEGORY_ID; // Data type that the shader can be applied to
	string_type shader_name = "";				       // The name of the shader program
	string_type shader_description = "";			   // A descripriton of the shader program

	bool update_shader_variables() {
		if (!shader_program_id) return false;

		for (shader_parameter_variable_struct_type variable : variables) {
			shader_manager.shader.set_f1(shader_program_id, variable.value, variable.variable_name);
		}

		for (shader_parameter_int_variable_struct_type int_variable : int_variables) {
			shader_manager.shader.set_i1(shader_program_id, int_variable.value, int_variable.variable_name);
		}

		for (shader_parameter_bool_variable_struct_type bool_variable : bool_variables) {
			shader_manager.shader.set_b1(shader_program_id, bool_variable.value, bool_variable.variable_name);
		}


		// This method of storing shader uniform variables as an array has been  problematic and is not used :
		// potential for future use ???????
/*		for (application_default_shader_uniform_variables_struct_type uniform_variable : default_shader_uniform_variables) {
if(uniform_variable.type == application_default_shader_variable_type_enum::Float1) printf("update_shader_variables :: uniform_variable type Float1 \n");
printf("update_shader_variables :: uniform_variable %s : %f : \n", uniform_variable.name.c_str(), *(float*)uniform_variable.value0);
			update_shader_variable(uniform_variable);
		}
*/
		return true;
	}

		// Compilor has linking problems with using glm vec1, ivec1 and uvec1 datatypes
		// and also with attempting to use std vector ::  suspect problems with glProgramUniform1fv 
		// or GLM as including headers directly makes no difference

	void update_shader_variable(application_default_shader_uniform_variables_struct_type uniform_variable) {
		switch (uniform_variable.type) {
				case Float1: shader_manager.shader.set_f1(shader_program_id, *(float*)uniform_variable.value0, uniform_variable.name); break;
				case Float2: shader_manager.shader.set_f2(shader_program_id, *(float*)uniform_variable.value0, *(float*)uniform_variable.value1,uniform_variable.name);break;
				case Float3: shader_manager.shader.set_f3(shader_program_id, *(float*)uniform_variable.value0, *(float*)uniform_variable.value1, 
																			 *(float*)uniform_variable.value2, uniform_variable.name);break;
				case Float4: shader_manager.shader.set_f4(shader_program_id, *(float*)uniform_variable.value0, *(float*)uniform_variable.value1, 
																			 *(float*)uniform_variable.value2, *(float*)uniform_variable.value3,uniform_variable.name);break;

//				case Floatv1: shader_manager.shader.set_vec1(shader_program_id, *(glm::vec1*)uniform_variable.value0,uniform_variable.name);break;
//				case Floatv1: shader_manager.shader.set_vec1(shader_program_id, *(std::vector<float> *)uniform_variable.value0,uniform_variable.name);break;
				case Floatv2: shader_manager.shader.set_vec2(shader_program_id, *(glm::vec2*)uniform_variable.value0,uniform_variable.name);break;
				case Floatv3: shader_manager.shader.set_vec3(shader_program_id, *(glm::vec3*)uniform_variable.value0,uniform_variable.name);break;
				case Floatv4: shader_manager.shader.set_vec4(shader_program_id, *(glm::vec4*)uniform_variable.value0, uniform_variable.name); break;

				case Bool://printf("update_shader_variable : %i\n", *(int*)uniform_variable.value0);
				case Int1: shader_manager.shader.set_i1(shader_program_id, *(int*)uniform_variable.value0, uniform_variable.name); break;
				case Int2: shader_manager.shader.set_i2(shader_program_id, *(int*)uniform_variable.value0, *(int*)uniform_variable.value1, uniform_variable.name); break;
				case Int3: shader_manager.shader.set_i3(shader_program_id, *(int*)uniform_variable.value0, *(int*)uniform_variable.value1, 
																		   *(int*)uniform_variable.value2, uniform_variable.name);break;
				case Int4: shader_manager.shader.set_i4(shader_program_id, *(int*)uniform_variable.value0, *(int*)uniform_variable.value1, 
																		   *(int*)uniform_variable.value2, *(int*)uniform_variable.value3,uniform_variable.name);break;

//				case Intv1: shader_manager.shader.set_ivec1(shader_program_id, *(glm::ivec1*)uniform_variable.value0,uniform_variable.name);break;
//				case Intv1: shader_manager.shader.set_ivec1(shader_program_id, *(std::vector<int> *)uniform_variable.value0,uniform_variable.name);break;
				case Intv2: shader_manager.shader.set_ivec2(shader_program_id, *(glm::ivec2*)uniform_variable.value0,uniform_variable.name);break;
				case Intv3: shader_manager.shader.set_ivec3(shader_program_id, *(glm::ivec3*)uniform_variable.value0, uniform_variable.name); break;
				case Intv4: shader_manager.shader.set_ivec4(shader_program_id, *(glm::ivec4*)uniform_variable.value0,uniform_variable.name);break; 

				case Mat2fv:   shader_manager.shader.set_mat2(shader_program_id,   *(glm::mat2*)uniform_variable.value0,uniform_variable.name);break;
				case Mat2x3fv: shader_manager.shader.set_mat2x3(shader_program_id, *(glm::mat2x3*)uniform_variable.value0,uniform_variable.name);break;
				case Mat2x4fv: shader_manager.shader.set_mat2x4(shader_program_id, *(glm::mat2x4*)uniform_variable.value0,uniform_variable.name);break;
				case Mat3fv:   shader_manager.shader.set_mat3(shader_program_id,   *(glm::mat3*)uniform_variable.value0,uniform_variable.name);break;
				case Mat3x2fv: shader_manager.shader.set_mat3x2(shader_program_id, *(glm::mat3x2*)uniform_variable.value0,uniform_variable.name);break;
				case Mat3x4fv: shader_manager.shader.set_mat3x4(shader_program_id, *(glm::mat3x4*)uniform_variable.value0,uniform_variable.name);break;
				case Mat4fv:   shader_manager.shader.set_mat4(shader_program_id,   *(glm::mat4*)uniform_variable.value0,uniform_variable.name);break;
				case Mat4x2fv: shader_manager.shader.set_mat4x2(shader_program_id, *(glm::mat4x2*)uniform_variable.value0,uniform_variable.name);break;
				case Mat4x3fv: shader_manager.shader.set_mat4x3(shader_program_id, *(glm::mat4x3*)uniform_variable.value0,uniform_variable.name);break;

				case UInt1: shader_manager.shader.set_ui1(shader_program_id, *(unsigned int*)uniform_variable.value0, uniform_variable.name); break;
				case UInt2: shader_manager.shader.set_ui2(shader_program_id, *(unsigned int*)uniform_variable.value0, *(unsigned int*)uniform_variable.value1, uniform_variable.name); break;
				case UInt3: shader_manager.shader.set_ui3(shader_program_id, *(unsigned int*)uniform_variable.value0, *(unsigned int*)uniform_variable.value1, 
																		     *(unsigned int*)uniform_variable.value2, uniform_variable.name);break;
				case UInt4: shader_manager.shader.set_ui4(shader_program_id, *(unsigned int*)uniform_variable.value0, *(unsigned int*)uniform_variable.value1, 
																		     *(unsigned int*)uniform_variable.value2, *(unsigned int*)uniform_variable.value3,uniform_variable.name);break;

//				case UIntv1: shader_manager.shader.set_uivec1(shader_program_id, *(glm::uvec1*)uniform_variable.value0,uniform_variable.name);break;
//				case UIntv1: shader_manager.shader.set_uivec1(shader_program_id, *(std::vector<unsigned int> *)uniform_variable.value0,uniform_variable.name);break;
				case UIntv2: shader_manager.shader.set_uivec2(shader_program_id, *(glm::uvec2*)uniform_variable.value0,uniform_variable.name);break;
				case UIntv3: shader_manager.shader.set_uivec3(shader_program_id, *(glm::uvec3*)uniform_variable.value0,uniform_variable.name);break;
				case UIntv4: shader_manager.shader.set_uivec4(shader_program_id, *(glm::uvec4*)uniform_variable.value0,uniform_variable.name);break;

		}
	}

private:
	shader_db_manager_class shader_manager;
};


#include "../Scene/Scene_objects/camera_object.h"
#include "../Scene/Scene_objects/light_object.h"
struct universal_shader_variables_struct_type {
	// Default shader varaibles present in all shaders to be placed below

	camera_object_class               *camera;
	//std::vector <light_object_class> scene_lights; Yet to be implemented

	light_object_class *default_light;

	float     ambience = 0.3;
	float     specular_strength = 0.3;
	bool      camera_as_light = false;
	int       type_camera_light = 0;
	glm::vec3 camera_light_offset = { 0.0,0.0,0.0 };
	glm::vec3 lighting_direction  = { 0.0,0.0,0.0 };
	glm::vec4 light_color         = { 1.0,1.0,1.0,1.0 };

	float      light_intensity = 0.5;

	int        t_frame = 0;

	void update_default_shader_variables(idu_type shader_program_id) {
		camera->update(shader_program_id);
		default_light->update(shader_program_id);

		shader_manager.shader.set_f1(shader_program_id,   ambience, "ambience");
		shader_manager.shader.set_f1(shader_program_id,   specular_strength, "specular_strength");
		shader_manager.shader.set_f1(shader_program_id,   light_intensity, "lighting_intensity");
		shader_manager.shader.set_vec3(shader_program_id, lighting_direction, "lighting_direction");
		shader_manager.shader.set_vec4(shader_program_id, light_color, "light_color");

		shader_manager.shader.set_i1(shader_program_id,   type_camera_light, "camera_lighting_type");
		shader_manager.shader.set_b1(shader_program_id,   camera_as_light, "use_camera_lighting");
		shader_manager.shader.set_vec3(shader_program_id, camera_light_offset, "lighting_camera_offset");

		shader_manager.shader.set_i1(shader_program_id, t_frame, "t_frame");
/*
		// Get the location of the attributes that enters in the vertex shader
		GLint position_attribute = glGetAttribLocation(shader_program_id, "aPosition");

		// Specify how the data for position can be accessed
		glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Enable the attribute
		glEnableVertexAttribArray(position_attribute);
*/
	}


private:
	shader_db_manager_class shader_manager;
};