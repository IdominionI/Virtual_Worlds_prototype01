#pragma once

#include <Source/Editor/Scene/scene_manager.h>

#include <Source/Editor/Main_Window/Widgets/parameters_widget.h>

//#include "../../Render/hex_surface_render.h"

// Including any of the following generates linking errors unkown why
#include "hex_generation_selection_widget.h"
#include "hex_shaders_widget.h"
#include "hex_automata_widget.h"

/*
				Hex surface paramters widget

	This widget class is a parent widget that manages the display
	of hex generation, shader variable and cellula automata child
	widgets via selection of ImGui tabs.

	This widget class is thus the widget class that is called by
	the main application editor to display hex surface parameter
	data to be changed or generate data.
*/

class hex_surface_parameters_widget_class : public parameters_widget_category_basis_class {
public:
	scene_manager_class *scene_manager = NULL;
	log_panel_class     *log_panel     = NULL;

	void display_parameters_widget(id_type current_selected_object_id);

private:
	hex_surface_shaders_widget_class    hex_surface_shaders_widget;
	hex_surface_automata_widget_class	hex_surface_automata_widget;

	hex_surface_generation_selection_widget_class hex_surface_generation_selection_widget;


	void display_hex_surface_generation_selection_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id);
	void display_hex_surface_shader_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id);
	void display_hex_surface_automata_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id);

};