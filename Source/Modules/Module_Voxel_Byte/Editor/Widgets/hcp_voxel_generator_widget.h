#pragma once

#include <Source/Editor/Scene/scene_manager.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include "../../Voxel_hcp_object/DataTypes/dt_voxel_generator.h"
#include "../../Kernal/voxel_function_import_export.h"

#include "hcp_voxel_generator_variables_widget.h"
#include "../../Compute/voxel_compute_generation.h"

#include "../../Render/voxel_hcp_render.h"
/*
		HCP Voxel Volume generation widget class

	This class widget defines an ImGui widget and child widgets
	that are used to define the parameters and compute shader
	variables required to generate the values that define a
	3D HCP Voxel Volume.

	This class widget has controls to generate the 3D HCP Voxel
	Volume and display the results on the computer screen in
	incremental steps by changing the shader variable values
	according to the settings that the user defines for each
	compute shader variable.
*/

class voxel_hcp_generation_widget_class {
public:
	generator_variables_widget_class  generator_variables_widget_class;
	log_panel_class				     *log_panel = NULL;

	float voxel_scale_value = 1.0f;
	
	id_type                  current_selected_entity_id   = -1;  // entity id of the selected entity to display/modify
	voxel_hcp_object_class  *voxel_hcp_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
	scene_manager_class     *scene_manager = NULL;


	void display();
	//void change_voxels_display();
	void perform_decrement_variables();
	void perform_increment_variables();
	void execute_voxel_function(bool notification = true);
	void initialise_parameters();
	void initialise_invocation();
	//void update_voxel_size();
	void test_voxel_cell_coord_from_cartesian(voxel_object_data_class voxel_object_data);

private:
	int invocation = 4;

	struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };

	voxel_function_import_export_class voxel_function_import_export;
	voxel_compute_generator_class      voxel_generator;
	voxel_hcp_render_class             voxel_hcp_render;

	void define_voxel_generation_parameters();
	void get_expression_file();
	void save_generation_parameters();
	void load_generation_parameters();
	void clear_variables();

};