#pragma once

#include <vector>

#include "../Common/definitions.h"

#include "scene_graph_object.h"

/*
				Scene Node Class

	Class that defines a data structure to define 
	the storage node data element of a scene object
	that can be of any type, and manage the nodes
	data.

	The original data structure model was to be of a
	tree type structure with parent child realtionship,
	but problems beyond ability to solve downgraded the
	data storage model to a C++ vector list. Thus some 
	of the code may be redundant and not needed but kept
	with the view it may be needed at some time.
*/


/*
	failed to debug why in original tree model of scene graph the scene_node_entity_id 
	caused rendering geometry to be found to a null value and crash the app despite tests
	that data was defined correctly on setup.

	Try another attempt in the future : keep parent child relationship to facilitate this
*/

template <class T>
class scene_node_class {
public:
	scene_node_class() {
		//parent = parent_node;
	}

	~scene_node_class() {}

	id_type  scene_node_entity_id; // this is the same as the entity id stored in the entity list

	//scene_node_class               *parent;
	std::vector<scene_node_class *> children;

	scene_graph_object_class <T> scene_graph_object;

	bool active          = true;
	//bool children_active = true;


	bool add_scene_node(id_type scene_node_entity_id) {
		scene_node_class *new_scene_node = new scene_node_class;

		if(new_scene_node == NULL){
//printf("add_scene_node 01 : new_scene_node == NULL\n");
			return false;
		}
//printf("add_scene_node 02 :%i \n", scene_node_entity_id);
		new_scene_node->scene_node_entity_id = scene_node_entity_id;
//printf("add_scene_node 03 : %i \n", new_scene_node->scene_node_entity_id);
		add_child(new_scene_node);

		return true;
	}

	bool delete_scene_node(id_type scene_node_entity_id) {
		 int scene_node_to_delete_index = find_scene_node_index(scene_node_entity_id);

		if (scene_node_to_delete_index < 0 ) return false;

		delete_child(scene_node_to_delete_index);

		return true;
	}

	int find_scene_node_index(id_type node_entity_id ) {
//printf("find_scene_node_index 00 \n");

		for (int i = 0; i < children.size(); i++) {
			scene_node_class *child = children[i];
			if (child->scene_node_entity_id == node_entity_id)
				return i;
//printf("find_scene_node 22 \n");
		}

		return -1;
	}

	scene_node_class *find_scene_node(id_type node_entity_id ) {
//printf("find_scene_node 00 \n");

		for (int i = 0; i < children.size(); i++) {
			scene_node_class *child = children[i];
			if (child->scene_node_entity_id == node_entity_id)
				return child;
//printf("find_scene_node 22 \n");
		}

		return NULL;
	}

	scene_node_class *get_scene_entity_render_object(id_type scene_node_entity_id) {
		scene_node_class *scene_node= find_scene_node(scene_node_entity_id);
/*
if (scene_node == NULL) {
	printf("get_scene_entity_render_object 00 : scene_node == NULL\n not found scene_node_entity_id\n");
}else
	printf("get_scene_entity_render_object 00 : scene_node != NULL\n found scene_node_entity_id : %i\n", scene_node->scene_node_entity_id);
*/
		return scene_node;
	}

private:
	void add_child(scene_node_class *child) {
		children.push_back(child);
	}

	void delete_child(int scene_node_to_delete_index) {
		scene_node_class *scene_child_node_to_delete = children[scene_node_to_delete_index];
		children.erase(children.begin() + scene_node_to_delete_index);

		delete scene_child_node_to_delete;
	}

};

