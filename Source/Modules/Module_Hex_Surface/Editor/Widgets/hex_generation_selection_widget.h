#pragma once

#include "hex_generator_widget.h"

#include "cart_to_hex_generator.h"

class hex_surface_generation_selection_widget_class {
public:

	log_panel_class* log_panel = NULL;

	id_type                    current_selected_entity_id   = -1;  // entity id of the selected entity to display/modify
	hex_surface_object_class  *hex_surface_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
	scene_manager_class       *scene_manager = NULL;

	bool display_as_points = true;

	void display();
	void change_hex_display();
	void update_hex_size();

private:
	int hcp_generation_type = 0;

	float hex_scale_value = 1.0f;
	float min_vscale = 0.001f, max_vscale = 1.000f;

	cart_to_hex_widget_class            cart_to_hex_widget;
	hex_surface_generation_widget_class hex_surface_generation_widget;

	void display_cart_to_hex_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id);
	void display_hex_surface_generation_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id);

};
