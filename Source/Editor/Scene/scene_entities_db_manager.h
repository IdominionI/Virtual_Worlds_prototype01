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
/*			case ENTITY_CATEGORY_HCP_VOXEL: if (!voxel_hcp_scene_objects.delete_object(entity_id)) // Debugging code here delete when satisfied all is well
												printf("delete_entity ##### : !voxel_hcp_scene_objects.delete_object(entity_id) : %i\n", entity_id);
										   else
												printf("delete_entity ##### : voxel_hcp_scene_objects.delete_object(entity_id): %i\n", entity_id);
											if (get_voxel_hcp_entity_object(entity_id) != NULL) 
												printf("delete_entity ##### : get_voxel_hcp_entity_object(entity_id) != NULL: %i\n", entity_id);
											else {
												printf("delete_entity ##### : get_voxel_hcp_entity_object(entity_id) == NULL: %i\n", entity_id);
												return true;
											}*/
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

/*
	//T *get_entity_object(id_type entity_id, id_type entity_category_id) {
printf("scene_entities_db_manager_class :: get_voxel_hcp_object %i : %i \n", entity_id, entity_category_id);
		switch (entity_category_id) {
			case ENTITY_CATEGORY_HCP_VOXEL: {voxel_hcp_object_class *entity = voxel_hcp_scene_objects.get_voxel_hcp_object(entity_id);
											if(entity == NULL) 
												printf("scene_entities_db_manager_class :: entity == NULL ##### %i : %i \n", entity_id, entity_category_id);
											else {
												printf("scene_entities_db_manager_class :: entity != NULL ##### %i : %i \n", entity_id, entity_category_id);
												//return entity;
											}
			}
				break;
			//case ENTITY_CATEGORY_HEX_SURF  : not yet implemented break;
		}
	}
*/
private:
};