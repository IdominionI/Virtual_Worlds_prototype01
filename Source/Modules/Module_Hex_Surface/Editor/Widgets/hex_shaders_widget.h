#pragma once
#include <Source/Graphics_Engine/Shader/shader_parameters.h>

#include <Source/Editor/Shader/shader_import_export.h>

#include "hex_shader_variables_widget.h"

#include "../../Hex_surface_object/hex_surface_object.h"
#include "../../Kernal/hex_surface_function_import_export.h"

#include "hex_generation_selection_widget.h"

/*
			Hex Surface shader widget class

	This class widget defines an ImGui widget and child widgets
	that are used to define the parameters and shader variables
	required to display a 2D hexagonal surface.

	This class widget has controls to change the display of the
	2D hexagonal surface on the computer screen in incremental 
	steps by changing the shader variable values according to 
	the settings that the user defines for each shader variable.
*/

class hex_surface_shaders_widget_class {
public:
	material_struct_type                *hex_surface_shader_parameters = NULL;
	hex_shader_variables_widget_class    shader_variables_widget; //****
	hex_surface_generation_selection_widget_class *hex_surface_generation_widget;

	log_panel_class                     *log_panel = NULL;

	id_type                   current_selected_entity_id    = -1;   // entity id of the selected entity to display/modify
	hex_surface_object_class *hex_surface_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	scene_manager_class      *scene_manager = NULL;

	void display();
	void perform_decrement_variables();
	void perform_increment_variables();
	void update_shader_variables(); // not sure this is needed as render node does this task
	void update_hex_shaders(bool notification = true);
	bool initialise_parameters();

private:
	voxel_texture_import_export_class  hex_surface_texture_import_export_data;
	hex_surface_render_class	       hex_surface_render;

	bool display_bounding_area;

	void save_shader_parameters();
	void load_shader_parameters();
	void select_vertex_shader_file();
	void select_point_geometry_shader_file();
	void select_fragment_shader_file();
	void clear_variables();

};
