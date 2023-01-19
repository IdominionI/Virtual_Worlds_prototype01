#pragma once

#include <Source/Graphics_Engine/Scene/scene_graph_manager.h>

// -------------------------------------------------------
#include "scene_render_object_manager.h" // *****
#include "scene_entities_db_manager.h"
#include "../Main_Window/Panels/log_panel.h"

/*
				SCENE MANAGER CLASS

	Manager class to act as gateway to perform functions or retrieve data
	on entity scene objects	required by the application, and to render
	scene objects onto the computer screen.
*/

//#define BOUNDING_GEOMETRY_OFFSET 1000000

class scene_manager_class {
public:
	scene_graph_manager_class       scene_graph_manager;
	scene_entities_db_manager_class entities_manager;

	scene_render_objects_manager_class render_objects_manager;

	log_panel_class* log_panel = NULL;



	scene_manager_class() {
		// At this time all bounding volumes use the same glsl program 
		/*
		bounding_volume.initialise();
		if (bounding_volume.program_id < 1)
			printf("!bounding_volume.initialise()\n");
		else
			printf("bounding_volume.initialiseed!!!!\n");
		*/
	}

	~scene_manager_class() {}

	void render_scene(universal_shader_variables_struct_type* universal_shader_variables) {
		scene_graph_manager.render_scene(universal_shader_variables);
	}

	scene_node_class <render_object_class>* get_scene_entity_render_object(id_type render_id) {
		return scene_graph_manager.get_scene_entity_render_object(render_id);
	}


	bool add_entity(id_type entity_id, id_type entity_category_id) {

		if (!entities_manager.add_new_entity(entity_id, entity_category_id)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not add entity of id %i to entity manager\n", entity_id);
			return false;
		}

		if (!scene_graph_manager.add_scene_entity_render_object(entity_id)) {
			entities_manager.delete_entity(entity_id, entity_category_id);
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not create scene render node to render data for entity of id  %i :\n", entity_id);
			return false;
		}

		scene_node_class <render_object_class>* entity_render_object = get_render_object(entity_id);

		if (entity_render_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Entity render object of id  %i  not found to render data\n", entity_id);
			return false;
		}

		return define_render_object(entity_id, entity_render_object, entity_category_id);

		return true;
	}

	bool delete_entity(id_type entity_id, id_type entity_category_id) {
//printf("Scene Manager : 00000 %i :\n", entity_id);//message to log
		if (!entities_manager.delete_entity(entity_id, entity_category_id)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Failed to delete scene entity of category id : %i and entity id %i :\n", entity_category_id, entity_id);
			return false;
		}

		// If entity has a bounding box render component, delete it. If do not will have the crash
		if (scene_graph_manager.entity_render_object_exists(entity_id + BOUNDING_GEOMETRY_OFFSET))
			scene_graph_manager.delete_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET);

		if (!scene_graph_manager.delete_scene_entity_render_object(entity_id)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Failed to deletion scene render entity of category id : %i and entity id %i :", entity_category_id, entity_id);
			return false;
		}

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Scene Manager : Deleted scene entity of category id : %i and entity id  \n", entity_category_id, entity_id);
		return true;
	}

	scene_node_class <render_object_class>* get_render_object(id_type entity_id) {
		return scene_graph_manager.get_scene_entity_render_object(entity_id);
	}

	void update_shader(scene_node_class <render_object_class>* entity_render_object, id_type category) {

		render_objects_manager.update_shader(entity_render_object, category);
	}

	// ++++++ DEFINE MODULE SCENE ROUTINES HERE ++++++

	object_basis_class *get_entity_object(id_type entity_id, id_type entity_category_id) {
		return entities_manager.get_entity_of_category(entity_id, entity_category_id);
	}

	bool define_render_object(id_type entity_id, scene_node_class <render_object_class>* entity_render_object, id_type entity_category_id) {
		object_basis_class *entity_object = get_entity_object(entity_id, entity_category_id);

		if (entity_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : entity object of id  %i and category %i not found\n", entity_id, entity_category_id);
			return false;
		}

		if (render_objects_manager.define_render_object(entity_render_object, entity_object, entity_category_id)) {

//printf("render_objects_manager.define_render_object(entity_render_object, entity_object, entity_category_id) \n");
			render_objects_manager.define_bounding_box(&scene_graph_manager, entity_id, entity_object, entity_category_id);
			return true;
		}

		return false;
	}




private:

};

