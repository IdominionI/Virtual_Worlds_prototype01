#pragma once

// Define module entity data type includes here that are to used in the application
#include <Source/Modules/Module_Voxel_Byte/Editor/Scene/voxel_hcp_scene_objects.h>
#include <Source/Modules/Module_Hex_Surface/Editor/Scene/hex_surface_scene_objects.h>

/*
				SCENE ENTITY DB MANAGER CLASS

	Manager class to perform functions to entity scene objects 
	required by the application, or to retrieve data of a 
	particular entity id of a given entity data type.
*/
// ********

#include "objects_category_basis.h"

class scene_entities_db_manager_class {
public : 
	~scene_entities_db_manager_class() {
		clear_scene_entities_db();
	}

	std::vector<scene_objects_category_class*> scene_objects;

	log_panel_class *log_panel = NULL;

	bool define_new_entity_category(id_type entity_category_id) {
		if (entity_category_id > -1) {

			int index = get_objects_of_category_index(entity_category_id);
			if (index >= 0) {
				printf("ERROR : Cannot define new Object category as it exists\n");
				return false;
			}

			scene_objects_category_class *scene_objects_of_category = new scene_objects_category_class;
			if (scene_objects_of_category == NULL) {
				printf("ERROR : Cannot define new Object category : memory allocation error\n");
				return false;
			}

			scene_objects_of_category->objects_category_id = entity_category_id;
			scene_objects.push_back(scene_objects_of_category);
printf("scene_entities_db_manager2_class:define_new_entity_category : %i\n", scene_objects.size());
			return true;
		}
		return false;
	}

	bool add_new_entity(id_type entity_id, id_type entity_category_id) {

		if (entity_category_id > -1 && entity_id > -1) {

			int index = get_objects_of_category_index(entity_category_id);

			if (index < 0) {
				printf("scene_entities_db_manager2_class : add_new_entity :: scene_objects_of_category == NULL\n");
				return false;
			}

printf("scene_entities_db_manager2_class : add_new_entity :: %i : %i  \n", scene_objects[index]->objects_category_id, entity_id);
			return scene_objects[index]->add_new_object(entity_id);
		}

		return false;
	}

	bool delete_entity(id_type entity_id, id_type entity_category_id) {
		if (entity_category_id > -1 && entity_id > -1) {
			int index = get_objects_of_category_index(entity_category_id);
			if (index < 0) {
				// error message
				return false;
			}

			return scene_objects[index]->delete_object(entity_id);
		}

		return false;
	}

	//void *get_entity_of_category(id_type entity_id, id_type entity_category_id){
	object_basis_class *get_entity_of_category(id_type entity_id, id_type entity_category_id){
		if (entity_category_id > -1 && entity_id > -1) {
			int index = get_objects_of_category_index(entity_category_id);
			if (index < 0) {
printf("scene_entities_db_manager2_class : get_entity_of_category :: scene_objects_of_category == NULL\n");
				return NULL;
			}

			return scene_objects[index]->get_entity_object(entity_id);
		}

		return NULL;
	}

	/*
	// The folowing function just seems not to work as returning a type scene_objects_category_class * does not
	//  seem to point to the vector entry of scene_objects as expected to be worked upon !!!!!!
	scene_objects_category_class *get_objects_of_category(id_type entity_category_id) {
		for(scene_objects_category_class *scene_objects_of_category: scene_objects){
printf("scene_entities_db_manager2_class : get_objects_of_category 1111: %i :: %i \n", scene_objects_of_category->objects_category_id, entity_category_id);
			if (scene_objects_of_category->objects_category_id == entity_category_id)
				return scene_objects_of_category;
		}

		return NULL;
	}
	*/

	int get_objects_of_category_index(id_type entity_category_id) {

//printf("scene_entities_db_manager2_class : get_objects_of_category_index 0000 : %i\n", scene_objects.size());
		for (int i = 0; i < scene_objects.size(); i++) {
//printf("scene_entities_db_manager2_class : get_objects_of_category_index 1111: %i :: %i \n", scene_objects[i]->objects_category_id, entity_category_id);
			if (scene_objects[i]->objects_category_id == entity_category_id)
				return i;
		}

		return -1;
	}

	void delete_entities_of_category(id_type entity_category_id) {
		if (entity_category_id > -1) {
			int index = get_objects_of_category_index(entity_category_id);
			if (index < 0) {
				// error message
				return ;
			}

			scene_objects[index]->delete_objects();
		}
	}

	void clear_scene_entities_db() {
		for (scene_objects_category_class* objects_of_category : scene_objects) {
			objects_of_category->delete_objects();
		}

		scene_objects.clear();
	}

protected:
private :
};
// ********

/*
class scene_entities_db_manager_class {
public:
	scene_entities_db_manager_class() {}
	~scene_entities_db_manager_class() {}

	// Define render object data base classes here
	voxel_hcp_scene_objects_class    voxel_hcp_scene_objects;
	hex_surface_scene_objects_class  hex_surface_scene_objects;

	log_panel_class				  *log_panel = NULL;

	bool add_new_entity(id_type entity_id, id_type entity_category_id) {

		switch (entity_category_id) {
			case ENTITY_CATEGORY_HCP_VOXEL : return(voxel_hcp_scene_objects.add_new_object(entity_id));break;
			case ENTITY_CATEGORY_HEX_SURF  : return(hex_surface_scene_objects.add_new_object(entity_id)); break;
		}

		return false;
	}


	bool delete_entity(id_type entity_id, id_type entity_category_id) {
		switch (entity_category_id) {
			case ENTITY_CATEGORY_HCP_VOXEL : return(voxel_hcp_scene_objects.delete_object(entity_id)); break;

											break;
			case ENTITY_CATEGORY_HEX_SURF  : return(hex_surface_scene_objects.delete_object(entity_id)); break;
		}

		return false;
	}

	// Use of templates and void pointers unsuccessfull in a more general way of returning pointers to entity objects
	// So need an individual funtion fo reach data type
	voxel_hcp_object_class* get_voxel_hcp_entity_object(id_type entity_id) {
		return voxel_hcp_scene_objects.get_voxel_hcp_object(entity_id);
	}

	hex_surface_object_class *get_hex_surface_entity_object(id_type entity_id) {
		return hex_surface_scene_objects.get_hex_surface_object(entity_id);
	}


private:
};
*/