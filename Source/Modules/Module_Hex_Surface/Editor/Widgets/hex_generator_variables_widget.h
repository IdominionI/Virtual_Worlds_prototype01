#pragma once
#include "../../Hex_surface_object/DataTypes/dt_hex_surface_generator.h"

/*
		Hex surface compute shader variables widget class

	This class widget defines an ImGui widget and child widgets
	that are used to define compute shader variables required to
	generate the values that define a 2D hexagonal surface.

	This class widget has inputs that define incremental step
	values that each varaible can be changed by enabling the user
	to perform incremental step changes to the calculation of
	the surface interactively.
*/

class hex_surface_generator_variables_widget_class {
public:
	int current_selected_var_type_id = -1;
	int current_selected_var_id      = -1;

	int real_var_type_id = 1;
	int int_var_type_id  = 2;
	int bool_var_type_id = 3;

	void variables_manager(hex_surface_generator_parameters_struct_type& hex_surface_generator_parameters);
	void add_variable(int var_type_id, hex_surface_generator_parameters_struct_type& hex_surface_generator_parameters);
	void delete_variable(int var_type_id, int var_id, hex_surface_generator_parameters_struct_type& hex_surface_generator_parameters);

};