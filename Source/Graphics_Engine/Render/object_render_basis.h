#pragma once

#include <string>

#include <Source/Graphics_Engine/Shader/shader_manager.h>
#include <Source/Graphics_Engine/Scene/Scene_objects/render_object.h>
#include <Source/Graphics_Engine/Scene/scene_node.h>

#include <Source/Editor/Main_Window/Panels/log_panel.h>

class object_render_basis_class {
public:

	virtual bool define_render_object(scene_node_class <render_object_class>* entity_render_object, void *entity_object, log_panel_class* log_panel) { return false; }
	virtual bool update_geometry_data(void* geometry_data, scene_node_class <render_object_class>* entity_render_object, log_panel_class* log_panel) { return false; }
	virtual bool define_shader_program(scene_node_class <render_object_class>* entity_render_object, log_panel_class* log_panel) { return false; }


protected:
	bool define_initial_shader_program(scene_node_class <render_object_class>* entity_render_object, void* entity_object, log_panel_class* log_panel) { return false; }

private:
};