#pragma once

#include "scene_node.h"

#include "Scene_objects/camera_object.h"
#include "Scene_objects/light_object.h"
#include "Scene_objects/render_object.h"

/*
			Scene graphics manager class

	Manager class to handle the management of the storage
	and rendering of all geometry objects in the scene

	All scene object geometry and rendering data is defined
	by the scene_node_class class type that stores the scene
	geometry data and redering functionality for each 
	individual scene data object.
*/

class scene_graph_manager_class {
public :
	scene_graph_manager_class() {
		scene_root_render_object = new scene_node_class <render_object_class>;
	}

	~scene_graph_manager_class() {}

	//std::vector <camera_object_class>     scene_cameras; // I have more than one camera in the scene
	std::vector <light_object_class>        scene_lights;
	scene_node_class <render_object_class> *scene_root_render_object=NULL;

	void render_scene(universal_shader_variables_struct_type *universal_shader_variables) {
		// go to each scene_render_objectsscene_render_object
		// if geometry has changed then bind

		// Test to make sure all entity geometries are defined ???

		if (scene_root_render_object == NULL) {
			printf("Scene root node is undefined\n");
			return;
		}

		std::vector<scene_node_class<render_object_class>*> root_children = scene_root_render_object->children;

		for (int i = 0; i < root_children.size(); i++) {
			scene_node_class<render_object_class>* child = root_children[i];
//printf("render_scene 00 %i : %i \n", i, child->scene_node_entity_id);

			if (child != NULL){
//if(child->scene_graph_object.scene_object_class.geometry == NULL)
//printf("Scene_graph render_scene 11 : child->scene_graph_object.scene_object_class.geometry = NULL %i \n",child->scene_node_entity_id);
//else
//printf("Scene_graph render_scene 22 : child->scene_graph_object.scene_object_class.geometry != NULL %i \n", child->scene_node_entity_id);

				child->scene_graph_object.scene_object_class.render(universal_shader_variables);
			}
		}

	}

	bool add_scene_entity_render_object(id_type scene_node_entity_id) {
//printf("add_scene_entity_render_object AA %i :\n", scene_node_entity_id);
		return scene_root_render_object->add_scene_node(scene_node_entity_id);
	}

	bool delete_scene_entity_render_object(id_type scene_node_entity_id) {
//printf("delete_scene_entity_render_object DD %i :\n", scene_node_entity_id);
		return scene_root_render_object->delete_scene_node(scene_node_entity_id);
	}

	scene_node_class <render_object_class> *get_scene_entity_render_object(idu_type scene_node_entity_id) {
		return scene_root_render_object->get_scene_entity_render_object(scene_node_entity_id);
	}

private:

};
