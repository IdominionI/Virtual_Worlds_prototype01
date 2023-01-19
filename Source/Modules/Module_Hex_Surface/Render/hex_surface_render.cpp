#include "hex_surface_render.h"

//#include <Source/Graphics_Engine/Shader/shader_manager.h>

	bool hex_surface_render_class::define_hex_surface_render_object(scene_node_class <render_object_class> *entity_render_object,hex_surface_object_class *hex_surface_object, log_panel_class *log_panel) {
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

		//define_shader_variables(entity_render_object);// Is this needed ????

		if (!define_initial_shader_program(entity_render_object, hex_surface_object, log_panel)) return false;

		return true;
	}

	bool hex_surface_render_class::update_geometry_vertex_cloud_data(point_cloud3D_class *voxel_hcp_cloud,scene_node_class <render_object_class> *entity_render_object, log_panel_class *log_panel) {
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

//printf("hex_surface_render_class :: update_geometry_vertex_cloud_data @@@@ %i\n", point_cloud_data->vertices.size());

		if(!point_cloud_data->update_geometry()){
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not update geometry data : Could not create buffer(s) to store voxel point cloud data\n");
			return false;
		}

		return true;
	}

	bool hex_surface_render_class::define_shader_program(scene_node_class <render_object_class> *entity_render_object, log_panel_class *log_panel) {
		shader_format_class* shader_material = entity_render_object->scene_graph_object.scene_object_class.shader_material;

//printf("VERTEX SOURCE \n%s END VERTEX SOURCE\n",vw_vertex_shader.shader_code.c_str());
//printf("POINT SOURCE \n%s END POINT SOURCE\n", vw_point_geometry_shader.shader_code.c_str());

		// Copile the OpenGL shader and store the id reference to it to be used
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
//printf("define_shader_program 0000\n");

		shader_id = shader_db_manager.create_glsl_shader_program(*shader_material);

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

//----------------------------------------------------

	bool hex_surface_render_class::define_initial_shader_program(scene_node_class <render_object_class>* entity_render_object,hex_surface_object_class *hex_surface_object, log_panel_class *log_panel) {
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
			//printf("hex_surface_render_class: shader program not created\n");
			//printf("compile log \n %s \n", shader_db_manager.shader.compile_log.c_str());
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : hex surface shader program compilation failed\n");
			if (log_panel != NULL) log_panel->code_log.AddLog("INFO : Shader program compilation failed\n %s\n", shader_db_manager.shader.compile_log.c_str());
			return false;
		}
		else {
//printf("hex_surface_render_class : shader program created : %i\n", shader_id);
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_id);

			//entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id = shader_id;
			entity_render_object->scene_graph_object.scene_object_class.shader_material->shader_program_id = shader_id;
		}//shader program created


		// copy shader data used to render voxel object to voxel shader parameter data
		// Note : There are no existing user defined variables for default shader programs
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters = *shader_materialm;

		return true;
	}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
	bool hex_surface_render_object_class::define_render_object(object_basis_class *entity_object, scene_node_class <render_object_class>* entity_render_object) {
		hex_surface_object_class *hex_surface_object = dynamic_cast<hex_surface_object_class *>(entity_object);

		if (hex_surface_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Hex surface entity object not found\n");
			return false;
		}

		return hex_surface_render.define_hex_surface_render_object(entity_render_object, hex_surface_object, log_panel);
	}

	bool hex_surface_render_object_class::define_bounding_box(scene_graph_manager_class *scene_graph_manager, id_type entity_id, object_basis_class *entity_object) {
		hex_surface_object_class *hex_surface_object = dynamic_cast<hex_surface_object_class*>(entity_object);

		if (hex_surface_object == NULL) {
			printf("hex_surface_render_class :: define_bounding_box : hex_surface_object == NULL");
			return false;
		}

		// Define hcp voxel bounding volume render object
		if (bounding_volume.program_id > 0) {
			if (!scene_graph_manager->add_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET)) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not create entity bounding box for entity  %s :\n", hex_surface_object->object_name.c_str());
			}
			else {
				scene_node_class <render_object_class>* box_object = scene_graph_manager->get_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET);

				if (box_object == NULL) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find bounding box geometry data for voxel %s.\n", hex_surface_object->object_name.c_str());
					printf("hex_surface_render_class :: define_bounding_box : box_object == NULL.\n");
				}
				else {
					//printf("box_object != NULL.\n");
					line_class* bv = new line_class;
					material_struct_type* bm = new material_struct_type;

					bm->shader_program_id = bounding_volume.program_id;

					hex_surface_object->bounding_area.program_id = bounding_volume.program_id;
					hex_surface_object->bounding_area.geometry = bv;
					hex_surface_object->bounding_area.material = bm;

					box_object->scene_graph_object.scene_object_class.geometry = hex_surface_object->bounding_area.geometry;
					box_object->scene_graph_object.scene_object_class.geometry->init();
					box_object->scene_graph_object.scene_object_class.shader_material = hex_surface_object->bounding_area.material;

					//hex_surface_object->bounding_volume.update_limits(glm::vec3(-4.0, -2.5, 0.0), glm::vec3(1.0, 3, 0.0));// Testing only
					hex_surface_object->update_bounding_area();
				}
			}
		}

		return true;
	}

	bool hex_surface_render_object_class::update_shader(scene_node_class <render_object_class>* entity_render_object) {
		return hex_surface_render.define_shader_program(entity_render_object, log_panel);
	}
*/