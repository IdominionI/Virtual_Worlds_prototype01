#include "voxel_hcp_render_object.h"

/*
voxel_hcp_render_object_class::voxel_hcp_render_object_class() {
	// At this time all bounding volumes use the same glsl program 
	bounding_volume.initialise();
	if (bounding_volume.program_id < 1)
		printf("voxel_hcp_render_object_class bounding volume not initialise()\n");
	//else
	//	printf("bounding_volume.initialiseed!!!!\n");
}
*/

bool voxel_hcp_render_object_class::define_render_object(object_basis_class* entity_object, scene_node_class <render_object_class>* entity_render_object) {
	voxel_hcp_object_class* voxel_hcp_object = dynamic_cast<voxel_hcp_object_class*>(entity_object);

	if (voxel_hcp_object == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel entity object  not found\n");
		return false;
	}

	return voxel_hcp_render.define_voxel_hcp_render_object(entity_render_object, voxel_hcp_object, log_panel);
}

bool voxel_hcp_render_object_class::define_bounding_box(scene_graph_manager_class* scene_graph_manager, id_type entity_id, object_basis_class* entity_object) {
	voxel_hcp_object_class* voxel_hcp_object = dynamic_cast<voxel_hcp_object_class*>(entity_object);

	if (voxel_hcp_object == NULL) {
		printf("voxel_hcp_render_object :: define_bounding_box : voxel_hcp_object == NULL");
		return false;
	}

	// Define hcp voxel bounding volume render object
	if (bounding_volume.program_id > 0) {
		if (!scene_graph_manager->add_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not create entity bounding box for entity  %s :\n", voxel_hcp_object->object_name.c_str());
		}
		else {
			scene_node_class <render_object_class>* box_object = scene_graph_manager->get_scene_entity_render_object(entity_id + BOUNDING_GEOMETRY_OFFSET);

			if (box_object == NULL) {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find bounding box geometry data for voxel %s.\n", voxel_hcp_object->object_name.c_str());
				printf("voxel_hcp_render_object :: define_bounding_box : box_object == NULL.\n");
			}
			else {
				//printf("box_object != NULL.\n");
				line_class* bv = new line_class;
				material_struct_type* bm = new material_struct_type;// ****

				bm->shader_program_id = bounding_volume.program_id;// ****

				voxel_hcp_object->bounding_volume.program_id = bounding_volume.program_id;
				voxel_hcp_object->bounding_volume.geometry = bv;
				voxel_hcp_object->bounding_volume.material = bm;// ****		

				box_object->scene_graph_object.scene_object_class.geometry = voxel_hcp_object->bounding_volume.geometry;
				box_object->scene_graph_object.scene_object_class.geometry->init();

				box_object->scene_graph_object.scene_object_class.shader_material = voxel_hcp_object->bounding_volume.material;// ****

				//if(box_object->scene_graph_object.scene_object_class.shader_material == NULL)
				//printf("box_object->scene_graph_object.scene_object_class.shader_material == NULL\n");
				//else
				//printf("box_object->scene_graph_object.scene_object_class.shader_material != NULL : %i\n", bounding_volume.program_id);

									//voxel_hcp_object->bounding_volume.update_limits(glm::vec3(-4.0, -2.5, -5.0), glm::vec3(1.0, 3, 2.0));// Testing only
				voxel_hcp_object->update_bounding_volume();
				return true;
			}
		}
	}

	return false;
}

bool voxel_hcp_render_object_class::update_shader(scene_node_class <render_object_class>* entity_render_object) {
	return voxel_hcp_render.define_shader_program(entity_render_object, log_panel);
}