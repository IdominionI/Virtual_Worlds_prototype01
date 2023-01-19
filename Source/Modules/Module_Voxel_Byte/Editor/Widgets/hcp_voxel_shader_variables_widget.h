#pragma once

#include <Universal/ImGui/imgui.h>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>

#include <Source/Graphics_Engine/Shader/shader_parameters.h>
#include <Source/Graphics_Engine/Shader/shader_components.h>

/*
		HCP voxel volume shader variables widget class

	This class widget defines an ImGui widget and child widgets
	that are used to define shader variables required to be used
	in the OpenGl glsl shader program code that is compiled to
	display HCP voxel volume data.

	This class widget has inputs that define incremental step
	values that each varaible can be changed by enabling the user
	to perform incremental step changes to the shader values that
	display the hex surface interactively.
*/

class shader_variables_widget_class {
public:
	shader_variables_widget_class() {
	}
	
	~shader_variables_widget_class() {
	}

	int current_selected_var_type_id = -1;
	int current_selected_var_id      = -1;

	int real_var_type_id = 1;
	int int_var_type_id  = 2;
	int bool_var_type_id = 3;

	bool variables_manager(material_struct_type* voxel_shader_parameters);
	void add_variable(int var_type_id, material_struct_type* voxel_shader_parameters);
	void delete_variable(int var_type_id, int var_id, material_struct_type* voxel_shader_parameters);

};