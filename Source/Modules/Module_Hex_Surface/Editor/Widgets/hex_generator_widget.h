#pragma once
#include <Source/Editor/Scene/scene_manager.h>

#include "../../Kernal/hex_surface_function_import_export.h"

#include "hex_generator_variables_widget.h"

#include "../../Compute/hex_surface_compute_generation.h"
#include "../../Render/hex_surface_render.h"


#include "../../Hex_surface_object/hex_surface_object.h"
/*
			Hex Surface generation widget class

	This class widget defines an ImGui widget and child widgets
	that are used to define the parameters and compute shader
	variables required to generate the values that define a
	2D hexagonal surface.

	This class widget has controls to generate the 2D hexagonal
	surface and display the results on the computer screen in 
	incremental steps by changing the shader variable values 
	according to the settings that the user defines for each 
	compute shader variable.
*/

class hex_surface_generation_widget_class {
public:
	hex_surface_generator_variables_widget_class  hex_surface_generator_variables_widget_class;
	log_panel_class				                 *log_panel = NULL;

	bool display_as_points = true;
	
	id_type                   current_selected_entity_id    = -1;  // entity id of the selected entity to display/modify
	hex_surface_object_class *hex_surface_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
	scene_manager_class      *scene_manager                 = NULL;

	float hex_scale_value = 1.0f;

	void display();
	void perform_decrement_variables();
	void perform_increment_variables();
	void execute_hex_surface_function(bool notification = true);
	void initialise_parameters();
	void initialise_invocation();

	// test_hex_hexagon_cell_coord_from_cartesian for testing only. Delete when complete
	//void test_hex_hexagon_cell_coord_from_cartesian(hex_surface_object_data_class hex_surface_object_data);

private:
	int invocation = 4;

	float min_vscale = 0.001f, max_vscale = 1.000f;

	struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };

	hex_surface_function_import_export_class hex_surface_function_import_export;
	hex_surface_compute_generator_class      hex_surface_compute_generator;
	hex_surface_render_class                 hex_surface_render;

	void define_voxel_generation_parameters();
	void get_expression_file();
	void save_generation_parameters();
	void load_generation_parameters();
	void clear_variables();

};