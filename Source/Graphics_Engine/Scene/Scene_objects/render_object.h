#pragma once

#include "../../Common/pch.h"

#include "../../Geometry/geometry_basis.h"
#include "../../Shader/shader_components.h"

#include "camera_object.h"
#include "light_object.h"

class render_object_class
{
public:

	bool                         visible = true;
	geometry_basis_class        *geometry=NULL;
	shader_component_struct_type shader_material; // 

	// Do rendering here and not in geometry as 
	//can acces both geometry and shader data more easily here
	bool render(universal_shader_variables_struct_type *universal_shader_variables){
//printf("render_object_class 00 %i\n", shader_material.shader_program_id);
		//If Include the following then only the last render object in the render object list will be displayed
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // wipe the drawing surface clear 

		if (!visible) return true;

		shader_manager.shader.use(shader_material.shader_program_id);

		// DEFINE WORLD SHADER VARIABELS HERE THAT IS COMMON TO EVERY SHADER PROGRAM
		if (universal_shader_variables != NULL) {
//printf("render_object_class :: render 0000: %i\n", shader_material.shader_program_id);
			universal_shader_variables->update_default_shader_variables(shader_material.shader_program_id);
		}

		// DEFINE SHADER VARIABLES
		//shader_manager.shader.set_vec3(shader_material.shader_program_id, { 1.0f, 0.0f, 0.0f }, "albedo");// test shader variable that could not presently be defined :: delete when testing complete
		shader_material.update_shader_variables();

//printf("33render_object_class 33\n");
		if(geometry!= NULL) geometry->render(shader_material.shader_program_id);
//printf("44render_object_class 44\n");
// 
		//shader_manager.release_shader(shader_material.shader_program_id);// Using this gives a blank screen !!!!! Wasted 2-3 hours to figure this out !!!!!
		return true;
	}


private:
	shader_db_manager_class shader_manager;
};

