#pragma once

#include <Source/Editor/Scene/scene_manager.h>
#include <Source/Editor/Main_Window/Widgets/parameters_widget.h>

//#include "../../Render/voxel_hcp_render.h"

// Including any of the following generates linking errors unkown why
#include "hcp_voxel_generation_selection_widget.h"
#include "hcp_voxel_shaders_widget.h"
#include "hcp_voxel_automata_widget.h"

/*
				HCP voxel volume paramters widget

	This widget class is a parent widget that manages the display
	of HCP voxel generation, shader variable and cellula automata child
	widgets via selection of ImGui tabs.

	This widget class is thus the widget class that is called by
	the main application editor to display hex surface parameter
	data to be changed or generate data.
*/

class voxel_hcp_parameters_widget_class : public parameters_widget_category_basis_class {
public:
	scene_manager_class *scene_manager = NULL;
	log_panel_class     *log_panel     = NULL;

	void display_parameters_widget(id_type current_selected_object_id);

private:
	voxel_shaders_widget_class		   voxel_shaders_widget;
	voxel_hcp_automata_widget_class	   voxel_hcp_automata_widget;

	hcp_voxel_genertion_selection_widget_class hcp_voxel_genertion_selection_widget;

	void display_voxel_generation_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id);
	void display_voxel_shader_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id);
	void display_voxel_automata_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id);

};
