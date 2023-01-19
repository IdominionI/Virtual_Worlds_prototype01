#pragma once

#include <Source/Editor/Scene/object_render_basis.h>

#include "hex_surface_render.h"

class hex_surface_render_object_class : public object_render_basis_class {
public:
	hex_surface_render_object_class() {
		// At this time all baounding volumes use the same glsl program 
		bounding_volume.initialise();

		if (bounding_volume.program_id < 1)
			printf("hex_surface_render_object_class bounding volume not initialise()\n");
		//else
		//	printf("bounding_volume.initialiseed!!!!\n");
	}

	bool define_render_object(object_basis_class* entity_object, scene_node_class <render_object_class>* entity_render_object);
	bool define_bounding_box(scene_graph_manager_class* scene_graph_manager, id_type entity_id, object_basis_class* entity_object);
	bool update_shader(scene_node_class <render_object_class>* entity_render_object);

private:
	hex_surface_render_class hex_surface_render;
	bounding_volume_class    bounding_volume;
};