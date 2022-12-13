#pragma once
#include <string>

#include <Source/Graphics_Engine/Shader/shader_manager.h>
#include <Source/Graphics_Engine/Scene/Scene_objects/render_object.h>
#include <Source/Graphics_Engine/Scene/scene_node.h>


#include <Source/Graphics_Engine/Scene/scene_graph_manager.h>

#include <Source/Editor/Main_Window/Panels/log_panel.h>

#define BOUNDING_GEOMETRY_OFFSET 1000000 // Need to place this in a more approriate location ?????

class object_render_basis_class {
public:
	id_type objects_category_id = -1;

	log_panel_class* log_panel = NULL;

	virtual bool define_render_object(object_basis_class *entity_object, scene_node_class <render_object_class>* entity_render_object) { return false; }
	virtual bool define_bounding_box(scene_graph_manager_class *scene_graph_manager, id_type entity_id, object_basis_class* entity_object) { return false; }
	virtual bool update_shader(scene_node_class <render_object_class>* entity_render_object) { return false; }
};