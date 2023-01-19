#pragma once

#include <Source/Editor/Shader/shader_import_export.h>

#include "hcp_voxel_shader_variables_widget.h"

#include "../../Voxel_hcp_object/voxel_hcp_object.h"
#include "../../Kernal/voxel_function_import_export.h"

#include "hcp_voxel_generation_selection_widget.h"
/*
			HCP voxel volume shader widget class

	This class widget defines an ImGui widget and child widgets
	that are used to define the parameters and shader variables
	required to display a 3D HCP voxel volume surface.

	This class widget has controls to change the display of the
	3D HCP voxel volume on the computer screen in incremental
	steps by changing the shader variable values according to
	the settings that the user defines for each shader variable.
*/

class voxel_shaders_widget_class {
public:
	material_struct_type              *voxel_shader_parameters = NULL;
	shader_variables_widget_class      shader_variables_widget;
	hcp_voxel_genertion_selection_widget_class *hcp_voxel_genertion_selection_widget;
	log_panel_class               *log_panel = NULL;

	//bool animate_shaders             = false;

	id_type                  current_selected_entity_id = -1;   // entity id of the selected entity to display/modify
	voxel_hcp_object_class  *voxel_hcp_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	scene_manager_class      *scene_manager = NULL;

	void display();
	void perform_decrement_variables();
	void perform_increment_variables();
	void update_shader_variables();
	void update_voxel_shaders(bool notification = true);
	bool initialise_parameters();

private:
	voxel_texture_import_export_class  voxel_texture_import_export_data;
	voxel_hcp_render_class			   voxel_hcp_render;

	bool display_bounding_volume = true;

	void save_shader_parameters();
	void load_shader_parameters();
	void select_vertex_shader_file();
	void select_point_geometry_shader_file();
	void select_geometry_shader_file();
	void select_fragment_shader_file();
	void clear_variables();

};
