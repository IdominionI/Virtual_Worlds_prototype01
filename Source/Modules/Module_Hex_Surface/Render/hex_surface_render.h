#pragma once

#include <string>

#include <Source/Graphics_Engine/Shader/shader_manager.h>
#include <Source/Graphics_Engine/Scene/Scene_objects/render_object.h>
#include <Source/Graphics_Engine/Scene/scene_node.h>

#include <Source/Graphics_Engine/Geometry/point_cloud.h>

#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include "../Hex_surface_object/hex_surface_object.h"

/*
			Hex Surface Render Class

	This class mamages the generation and deletion of the shader program
	that is to be used to display the 2D hex surface that an instance of
	this class is asociated with.

	The user selects through the application the files that contain the
	glsl code to be compiled and when an update to the shader program is
	called, a new shader program is compiled, and If successful, the older
	shader progrmam is destroyed, and the new shader program assigned to
	the hex surface object render variable.
*/

class hex_surface_render_class {
public:
	point_cloud3D_class  *voxel_hcp_cloud = NULL;
	material_struct_type *voxel_material  = NULL;

	std::string shader_basis_code_directory_pathname = "assets/Shaders/Default/Hex_surface/Shader_basis_code/";

	std::string default_shader_file_directory   = "assets/Shaders/Default/Hex_surface/";
	std::string shader_file_directory           = "assets/Shaders/Default/Hex_surface/";


	std::string voxel_hcp_default_vertex_file   = "default_hex_surface_HS_VS.glsl";
	std::string voxel_hcp_default_point_file    = "default_hex_surface_HS_PGS.glsl";
	std::string voxel_hcp_default_fragment_file = "default_hex_surface_HS_FS.glsl";

	std::string voxel_hcp_vertex_file           = "default_hex_surface_HS_VS.glsl";
	std::string voxel_hcp_point_file            = "default_hex_surface_HS_PGS.glsl";
	std::string voxel_hcp_fragment_file         = "default_hex_surface_HS_FS.glsl";

	bool define_hex_surface_render_object(scene_node_class <render_object_class> *entity_render_object,hex_surface_object_class *hex_surface_object, log_panel_class *log_panel) {
		voxel_hcp_cloud = new point_cloud3D_class;
		voxel_material  = new material_struct_type;

		if (voxel_hcp_cloud == NULL) {
			printf("ERROR : Failed to create point data cloud entry\n");
			return false;
		}

		// Future consideration is to have entity_render_object->scene_graph_object.scene_object_class.geometry = hex_surface_object_class.point_cloud
		// and then just update the buffers in render_object_class through entity_render_object->scene_graph_object.scene_object_class.geometry->update

		entity_render_object->scene_graph_object.scene_object_class.geometry = voxel_hcp_cloud;
		entity_render_object->scene_graph_object.scene_object_class.geometry->init();

		entity_render_object->scene_graph_object.scene_object_class.shader_material = voxel_material;

		define_shader_variables(entity_render_object);// Is this needed ????

		if (!define_initial_shader_program(entity_render_object, hex_surface_object, log_panel)) return false;

		return true;
	}

	bool update_geometry_vertex_cloud_data(point_cloud3D_class *voxel_hcp_cloud,scene_node_class <render_object_class> *entity_render_object, log_panel_class *log_panel) {
		if (voxel_hcp_cloud == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not update geometry data : Voxel point cloud not defined.\n");
			return false;
		}

		if (entity_render_object->scene_graph_object.scene_object_class.geometry == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not update geometry data : Geometry storage not defined\n");
			return false;
		}

		point_cloud3D_class *point_cloud_data = static_cast<point_cloud3D_class*>(entity_render_object->scene_graph_object.scene_object_class.geometry);

		point_cloud_data->vertices.clear();
		point_cloud_data->vertices = voxel_hcp_cloud->vertices;

//printf("voxel_hcp_render_class :: update_geometry_vertex_cloud_data @@@@ %i\n", point_cloud_data->vertices.size());

		if(!point_cloud_data->update_geometry()){
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not update geometry data : Could not create buffer(s) to store voxel point cloud data\n");
			return false;
		}

		return true;
	}

	bool define_shader_program(scene_node_class <render_object_class> *entity_render_object, log_panel_class *log_panel) {
		shader_format_class* shader_material = entity_render_object->scene_graph_object.scene_object_class.shader_material;

//printf("VERTEX SOURCE \n%s END VERTEX SOURCE\n",vw_vertex_shader.shader_code.c_str());
//printf("POINT SOURCE \n%s END POINT SOURCE\n", vw_point_geometry_shader.shader_code.c_str());

		// Copile the OpenGL shader and store the id reference to it to be used
		//shader_db_manager.remove_shader_program(entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id);
		//entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id = -1;

		// Define which shaders to use and the pathname to the shader snippit  code to define the shader
		if (shader_material->use_default_vertex_shader)
			shader_material->glsl_vertex_shader_file_pathname = shader_material->default_vertex_shader_file_pathname;
		else
			shader_material->glsl_vertex_shader_file_pathname = shader_material->vertex_shader_file_pathname;

		if (shader_material->use_point_shader) {
			if (shader_material->use_default_point_shader)
				shader_material->glsl_geometry_shader_file_pathname = shader_material->default_point_shader_file_pathname;
			else
				shader_material->glsl_geometry_shader_file_pathname = shader_material->point_shader_file_pathname;
		} else { // Use geometry shader
			if (shader_material->use_default_geometry_shader)
				shader_material->glsl_geometry_shader_file_pathname = shader_material->default_geometry_shader_file_pathname;
			else
				shader_material->glsl_geometry_shader_file_pathname = shader_material->geometry_shader_file_pathname;
		}

		if (shader_material->use_default_fragment_shader)
			shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;
		else
			shader_material->glsl_fragment_shader_file_pathname = shader_material->fragment_shader_file_pathname;

		GLuint shader_id = 0;
		//if (shader_material->use_point_shader) {
//printf("define_shader_program 0000\n");
			//shader_id = shader_db_manager.shader.compile_shader_program(vw_vertex_shader.shader_code, vw_point_geometry_shader.shader_code, vw_fragment_shader.shader_code);
			shader_id = shader_db_manager.create_glsl_shader_program(*shader_material);
		//} else {
//printf("define_shader_program 1111\n");
		//	shader_id = shader_db_manager.shader.compile_shader_program(vw_vertex_shader.shader_code, vw_geometry_shader.shader_code, vw_fragment_shader.shader_code);
		//}

		if (!shader_id) {// Shader compilation did not succeed
//printf("define_shader_program 22222\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :  Unable to compile shader program\n");

//printf("define_shader_program 22222BBBB \n");
//if(log_panel == NULL )printf("define_shader_program 22222BBBB log_panel == NULL");
			//log_panel->display_code_log_code(compile_code);
//printf("define_shader_program 22222CCCC\n");
			//log_panel->display_code_errors(shader_db_manager.shader.compile_log);
			log_panel->display_code_errors(shader_db_manager.shader.compile_log);
//printf("define_shader_program 22222DDDD\n");
			return false;
		} else {// Shader compilation successful
//printf("define_shader_program 33333 %i\n", shader_id);
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_id);

//printf("define_shader_program 44444 %i\n", entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id);
			shader_material->shader_program_id = shader_id;
//printf("define_shader_program 55555 %i\n", entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id);
		}//shader program created
		return true;
	}



	void define_shader_variables(scene_node_class <render_object_class> *entity_render_object) {
		// TO DO 
		/*
				shader_parameter_variable_struct_type v1, v2, v3;
				v1.variable_name = "roughness"; v1.value = 0.2f;
				v2.variable_name = "metallic"; v2.value = 0.1f;
				v3.variable_name = "ao"; v3.value = 1.0f;
				test_mesh->scene_graph_object.scene_object_class.shader_material.variables.push_back(v1);
				test_mesh->scene_graph_object.scene_object_class.shader_material.variables.push_back(v2);
				test_mesh->scene_graph_object.scene_object_class.shader_material.variables.push_back(v3);
		*/
	}

private:
	shader_db_manager_class shader_db_manager;

	shader_parameters_struct_type shader_parameters;

	std::string vertex_shader_file   = "";
	std::string geometry_shader_file = "";
	std::string fragment_shader_file = "";

	bool define_initial_shader_program(scene_node_class <render_object_class>* entity_render_object,hex_surface_object_class *hex_surface_object, log_panel_class *log_panel) {
		material_struct_type *shader_materialm = dynamic_cast<material_struct_type*>(entity_render_object->scene_graph_object.scene_object_class.shader_material);
		shader_format_class  *shader_material  = entity_render_object->scene_graph_object.scene_object_class.shader_material;

		shader_material->vertex_shader_file_pathname           = (default_shader_file_directory + voxel_hcp_default_vertex_file).c_str();
		shader_material->default_vertex_shader_file_pathname   = (default_shader_file_directory + voxel_hcp_default_vertex_file).c_str();
		shader_material->point_shader_file_pathname			   = (default_shader_file_directory + voxel_hcp_default_point_file).c_str();
		shader_material->default_point_shader_file_pathname    = (default_shader_file_directory + voxel_hcp_default_point_file).c_str();
		shader_material->fragment_shader_file_pathname		   = (default_shader_file_directory + voxel_hcp_default_fragment_file).c_str();
		shader_material->default_fragment_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_fragment_file).c_str();

//printf("define_initial_shader_program :Vertex glsl file \n%s\n"   , shader_material->default_vertex_shader_file_pathname.string().c_str() );
//printf("define_initial_shader_program :Geometry glsl file \n%s\n" , shader_material->default_point_shader_file_pathname.string().c_str());
//printf("define_initial_shader_program :Fragment glsl file \n%s\n" , shader_material->default_fragment_shader_file_pathname.string().c_str());
		// Copile the OpenGL shader and store the id reference to it to be used
		GLuint shader_id = 0;

		if (shader_material->use_point_shader) {
			shader_material->glsl_vertex_shader_file_pathname   = shader_material->default_vertex_shader_file_pathname;
			shader_material->glsl_geometry_shader_file_pathname = shader_material->default_point_shader_file_pathname;
			shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;

			shader_id = shader_db_manager.create_glsl_shader_program(*shader_material);
		}
		else {
			shader_material->glsl_vertex_shader_file_pathname   = shader_material->default_vertex_shader_file_pathname;
			shader_material->glsl_geometry_shader_file_pathname = shader_material->default_geometry_shader_file_pathname;
			shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;

			shader_id = shader_db_manager.create_glsl_shader_program(*shader_material);
		}

		if (!shader_id) {
			printf("hex_surface_render_class: shader program not created\n");
			printf("compile log \n %s \n", shader_db_manager.shader.compile_log.c_str());
			if (log_panel != NULL) log_panel->code_log.AddLog("INFO : Shader program compilation failed\n %s\n", shader_db_manager.shader.compile_log.c_str());
			return false;
		}
		else {
			printf("voxel_hcp_render_class : shader program created : %i\n", shader_id);
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_id);

			//entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id = shader_id;
			entity_render_object->scene_graph_object.scene_object_class.shader_material->shader_program_id = shader_id;
		}//shader program created


		// copy shader data used to render voxel object to voxel shader parameter data
		// Note : There are no existing user defined variables for default shader programs
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters = *shader_materialm;

		return true;
	}
};