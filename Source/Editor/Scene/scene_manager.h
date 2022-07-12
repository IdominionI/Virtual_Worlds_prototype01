#pragma once

#include <Source/Graphics_Engine/Scene/scene_graph_manager.h>
#include "scene_entities_db_manager.h"
#include "../Main_Window/Panels/log_panel.h"

// ++++++ DEFINE MODULE RENDER INCLUDE CLASSES HERE ++++++
#include <Source/Modules/Module_Voxel_Byte/Render/voxel_hcp_render.h>
#include <Source/Modules/Module_Hex_Surface/Render/hex_surface_render.h>

/*
				SCENE MANAGER CLASS

	Manager class to act as gateway to perform functions or retrieve data
	on entity scene objects	required by the application, and to render
	scene objects onto the computer screen.
*/

#define BOUNDING_GEOMETRY_OFFSET 1000000

class scene_manager_class {
public:
	scene_graph_manager_class       scene_graph_manager;
	scene_entities_db_manager_class entities_manager;
	log_panel_class				   *log_panel = NULL;

	scene_manager_class() {
		bounding_volume.initialise();//****
			//printf("!bounding_volume.initialise()");//****
	}

	~scene_manager_class() {}

	void render_scene(universal_shader_variables_struct_type *universal_shader_variables) {
		scene_graph_manager.render_scene(universal_shader_variables);
	}

	scene_node_class <render_object_class> *get_scene_entity_render_object(id_type render_id) {
		return scene_graph_manager.get_scene_entity_render_object(render_id);
	}


	bool add_entity(id_type entity_id, id_type entity_category_id)  {

		if (!entities_manager.add_new_entity(entity_id, entity_category_id)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not add entity of id %i to entity manager\n", entity_id);
			return false;
		}

		if (!scene_graph_manager.add_scene_entity_render_object(entity_id)) {
			entities_manager.delete_entity(entity_id, entity_category_id);
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not create scene render node to render data for entity of id  %i :\n", entity_id);
			return false;
		}

		scene_node_class <render_object_class> *entity_render_object = get_render_object(entity_id);

		if (entity_render_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Entity render object of id  %i  not found to render data\n", entity_id);
			return false;
		}

//if (voxel_hcp_object == NULL) {
//	if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Entity object of id  %i  not found\n", entity_id);
//	return false;
//}

		switch (entity_category_id) {
			case ENTITY_CATEGORY_HCP_VOXEL : return define_voxel_hcp_render_object(entity_id,entity_render_object); break;
			case ENTITY_CATEGORY_HCP_SURF  : return define_hex_render_object(entity_id, entity_render_object); break;
				
			// Add other entity object types here
		}

		return true;
	}

	// Test adding an entity to the scene graph : delete/modify when testing complete
	bool add_entity(idu_type entity_id) {
		return scene_graph_manager.add_scene_entity_render_object(entity_id);
	}

	bool delete_entity(id_type entity_id, id_type entity_category_id) {
//printf("Scene Manager : 00000 %i :\n", entity_id);//message to log
		if (!entities_manager.delete_entity(entity_id, entity_category_id)){
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Failed to delete scene entity of category id : %i and entity id %i :\n", entity_category_id,entity_id);
			return false;
		}

		// If entity has a bounding box render component, delete it. If do not will have the crash
		if(scene_graph_manager.entity_render_object_exists(entity_id + BOUNDING_GEOMETRY_OFFSET))
			scene_graph_manager.delete_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET);

		if (!scene_graph_manager.delete_scene_entity_render_object(entity_id)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Failed to deletion scene render entity of category id : %i and entity id %i :", entity_category_id, entity_id);
			return false;
		}

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Scene Manager : Deleted scene entity of category id : %i and entity id  \n", entity_category_id, entity_id);
		return true;
	}

	scene_node_class <render_object_class> *get_render_object(id_type entity_id) {
		return scene_graph_manager.get_scene_entity_render_object(entity_id);
	}

	void update_shader(scene_node_class <render_object_class> *entity_render_object, id_type category) {
		switch (category) {
			case ENTITY_CATEGORY_HCP_VOXEL : voxel_hcp_render.define_shader_program(entity_render_object, log_panel); break;
			case ENTITY_CATEGORY_HCP_SURF  : hex_surface_render.define_shader_program(entity_render_object, log_panel); break;

			// Other render object category types to go here as needed;
		}
	}

	// ++++++ DEFINE MODULE SCENE ROUTINES HERE ++++++

	// !!!!!!!!!!!!!!! hcp Voxel routines !!!!!!!!!!!!!!!!
	voxel_hcp_object_class *get_voxel_hcp_entity_object(id_type entity_id) {
		return entities_manager.get_voxel_hcp_entity_object(entity_id);
	}

	bool define_voxel_hcp_render_object(id_type entity_id, scene_node_class <render_object_class>* entity_render_object) {
		voxel_hcp_object_class* voxel_hcp_object = get_voxel_hcp_entity_object(entity_id);

		if (voxel_hcp_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel entity object of id  %i  not found\n", entity_id);
			return false;
		}

		if (!voxel_hcp_render.define_voxel_hcp_render_object(entity_render_object, voxel_hcp_object)) return false;

		define_voxel_hcp_bounding_volume_render_object(entity_id,voxel_hcp_object);
/*
		// Define hcp voxel bounding volume render object
		if (bounding_volume.program_id > 0) {
			if (!scene_graph_manager.add_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET)) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not create entity bounding box for entity  %s :\n", voxel_hcp_object->object_name.c_str());
			} else {
				scene_node_class <render_object_class> *box_object = scene_graph_manager.get_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET);

				if (box_object == NULL) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find bounding box geometry data for voxel %s.\n", voxel_hcp_object->object_name.c_str());
					printf("box_object == NULL.\n");
				}
				else {
					printf("box_object != NULL.\n");
					line_class *bv = new line_class;

					voxel_hcp_object->bounding_volume.program_id = bounding_volume.program_id;
					voxel_hcp_object->bounding_volume.geometry   = bv;

					box_object->scene_graph_object.scene_object_class.geometry = voxel_hcp_object->bounding_volume.geometry;
					box_object->scene_graph_object.scene_object_class.geometry->init();
					box_object->scene_graph_object.scene_object_class.shader_material.shader_program_id = bounding_volume.program_id;

					//voxel_hcp_object->bounding_volume.update_limits(glm::vec3(-4.0, -2.5, -5.0), glm::vec3(1.0, 3, 2.0));// Testing only
					voxel_hcp_object->update_bounding_volume();
				}
			}

		}
*/
		return true;
	}

	void define_voxel_hcp_bounding_volume_render_object(id_type entity_id,voxel_hcp_object_class *voxel_hcp_object) {
		// Define hcp voxel bounding volume render object
		if (bounding_volume.program_id > 0) {
			if (!scene_graph_manager.add_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET)) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not create entity bounding box for entity  %s :\n", voxel_hcp_object->object_name.c_str());
			}
			else {
				scene_node_class <render_object_class>* box_object = scene_graph_manager.get_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET);

				if (box_object == NULL) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find bounding box geometry data for voxel %s.\n", voxel_hcp_object->object_name.c_str());
					printf("box_object == NULL.\n");
				}
				else {
					printf("box_object != NULL.\n");
					line_class* bv = new line_class;

					voxel_hcp_object->bounding_volume.program_id = bounding_volume.program_id;
					voxel_hcp_object->bounding_volume.geometry = bv;

					box_object->scene_graph_object.scene_object_class.geometry = voxel_hcp_object->bounding_volume.geometry;
					box_object->scene_graph_object.scene_object_class.geometry->init();
					box_object->scene_graph_object.scene_object_class.shader_material.shader_program_id = bounding_volume.program_id;

					//voxel_hcp_object->bounding_volume.update_limits(glm::vec3(-4.0, -2.5, -5.0), glm::vec3(1.0, 3, 2.0));// Testing only
					voxel_hcp_object->update_bounding_volume();
				}
			}
		}
	}


	// !!!!!!!!!!!!!!! Hex surface routines !!!!!!!!!!!!!!!!
	hex_surface_object_class *get_hex_surface_entity_object(id_type entity_id) {
		return entities_manager.get_hex_surface_entity_object(entity_id);
		//return NULL;
	}

	bool define_hex_render_object(id_type entity_id, scene_node_class <render_object_class>* entity_render_object) {
		hex_surface_object_class *hex_surface_object = get_hex_surface_entity_object(entity_id);

		if (hex_surface_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Hex surface entity object of id  %i  not found\n", entity_id);
			return false;
		}

		if (!hex_surface_render.define_hex_surface_render_object(entity_render_object, hex_surface_object)) return false;

		define_hex_bounding_area_render_object(entity_id, hex_surface_object);

		return true;
	}

	void define_hex_bounding_area_render_object(id_type entity_id, hex_surface_object_class *hex_surface_object) {
		// Define hcp voxel bounding volume render object
		if (bounding_volume.program_id > 0) {
			if (!scene_graph_manager.add_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET)) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not create entity bounding box for entity  %s :\n", hex_surface_object->object_name.c_str());
			}
			else {
				scene_node_class <render_object_class>* box_object = scene_graph_manager.get_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET);

				if (box_object == NULL) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find bounding box geometry data for voxel %s.\n", hex_surface_object->object_name.c_str());
					printf("box_object == NULL.\n");
				}
				else {
					printf("box_object != NULL.\n");
					line_class* bv = new line_class;

					hex_surface_object->bounding_area.program_id = bounding_volume.program_id;
					hex_surface_object->bounding_area.geometry = bv;

					box_object->scene_graph_object.scene_object_class.geometry = hex_surface_object->bounding_area.geometry;
					box_object->scene_graph_object.scene_object_class.geometry->init();
					box_object->scene_graph_object.scene_object_class.shader_material.shader_program_id = bounding_volume.program_id;

					//hex_surface_object->bounding_volume.update_limits(glm::vec3(-4.0, -2.5, 0.0), glm::vec3(1.0, 3, 0.0));// Testing only
					hex_surface_object->update_bounding_area();
				}
			}
		}
	}

	// Add MODULE SCENE ROUTINES HERE


private:
	// ++++++ DEFINE MODULE RENDER CLASSES HERE ++++++
	voxel_hcp_render_class   voxel_hcp_render;
	hex_surface_render_class hex_surface_render;

	bounding_volume_class bounding_volume;//*****

	// Add MODULE render classes here
};