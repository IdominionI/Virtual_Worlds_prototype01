#pragma once

#include "../../Common/pch.h"

#include "../../Geometry/geometry_basis.h"
#include "../../Shader/shader_components.h"
#include "../../Shader/shader_format.h"


#include "camera_object.h"
#include "light_object.h"

/*
				 Render Object Class

	This is the Virtual Worlds class that stores and renders
	the geometry of any type of geometry data.

	Geometry data is defined and stored by the pointer to a 
	geometry class that inherits the geometry_basis_class.

	Shader data and the shader program id to display this 
	geometry data is stored in the shader_material structure
	variable.

	If the visible variable is set to true, the shader data in
	shader_material is queried, and with the vertex buffer object
	data stored in the geometry class object pointed to by the 
	geometry pointer variable, the shader program that is defined
	has the data imported into it and the object is set up to be
	redered by OpenGL to the screen.
	
	This method of scene rendering was inspired by QT QML View3D
	and by the BabylonCPP project found on github, but all the
	code here was written indepenently and with no reference to them.
*/

class render_object_class
{
public:

	bool                         visible         = true;
	geometry_basis_class        *geometry        = NULL;
	material_basis_struct_type  *shader_material = NULL;
	
	// Do rendering here and not in geometry as 
	// can acces both geometry and shader data more easily here
	bool render(universal_shader_variables_struct_type *universal_shader_variables){
//printf("render_object_class 00 %i\n", shader_material.shader_program_id);
		//If Include the following then only the last render object in the render object list will be displayed
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // wipe the drawing surface clear 
		if (shader_material == NULL) return true;
		if (!visible || shader_material->shader_program_id < 1) return true; // Need to return true or no other geometry object after this will be rendered

		shader_manager.shader.use(shader_material->shader_program_id);

		// DEFINE WORLD SHADER VARIABELS HERE THAT IS COMMON TO EVERY SHADER PROGRAM
		if (universal_shader_variables != NULL) {
//printf("render_object_class :: render 0000: %i\n", shader_material.shader_program_id);
			universal_shader_variables->update_default_shader_variables(shader_material->shader_program_id);
		}

		// DEFINE SHADER VARIABLES
//shader_manager.shader.set_vec3(shader_material.shader_program_id, { 1.0f, 0.0f, 0.0f }, "albedo");// test shader variable that could not presently be defined :: delete when testing complete
		shader_material->update_shader_variables();

//printf("33render_object_class 33\n");
		if(geometry!= NULL) geometry->render();
//printf("44render_object_class 44\n");

		//shader_manager.release_shader(shader_material.shader_program_id);// Using this gives a blank screen !!!!! Wasted 2-3 hours to figure this out !!!!!
		return true;
	}


private:
	shader_db_manager_class shader_manager;
};

