#include "hex_surface_render_object.h"



bool hex_surface_render_object_class::define_render_object(object_basis_class* entity_object, scene_node_class <render_object_class>* entity_render_object) {
	hex_surface_object_class* hex_surface_object = dynamic_cast<hex_surface_object_class*>(entity_object);

	if (hex_surface_object == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Hex surface entity object not found\n");
		return false;
	}

	return hex_surface_render.define_hex_surface_render_object(entity_render_object, hex_surface_object, log_panel);
}

bool hex_surface_render_object_class::define_bounding_box(scene_graph_manager_class* scene_graph_manager, id_type entity_id, object_basis_class* entity_object) {
	hex_surface_object_class* hex_surface_object = dynamic_cast<hex_surface_object_class*>(entity_object);

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