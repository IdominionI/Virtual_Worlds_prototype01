#pragma once

#include "hcp_voxel_generator_widget.h"

#include "cart_to_voxel_generator.h"

class hcp_voxel_genertion_selection_widget_class {
public:
	scene_manager_class *scene_manager = NULL;
	log_panel_class     *log_panel     = NULL;

	id_type                  current_selected_entity_id   = -1;  // entity id of the selected entity to display/modify
	voxel_hcp_object_class  *voxel_hcp_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	bool display_as_points = true;

	void display();
	void change_voxels_display();
	void update_voxel_size();

private:
	int hcp_generation_type = 0;

	float voxel_scale_value = 1.0f;
	float min_vscale = 0.001f, max_vscale = 1.000f;

	cart_to_voxel_widget_class		  cart_to_voxel_widget;
	voxel_hcp_generation_widget_class voxel_hcp_generation_widget;

	void display_cart_to_voxel_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id);
	void display_voxel_generation_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id);

};