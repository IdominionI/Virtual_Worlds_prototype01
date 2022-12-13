#pragma once
#include <vector>

#include "object_render_basis.h"


class scene_render_objects_manager_class {
public:
	~scene_render_objects_manager_class() {
		clear_scene_entities_db();
	}

	std::vector<object_render_basis_class*> scene_render_objects;

	log_panel_class* log_panel = NULL;

	bool define_new_entity_render_category(id_type entity_category_id) {
		if (entity_category_id > -1) {

			int index = get_objects_of_category_index(entity_category_id);
			if (index >= 0) {
				printf("ERROR : Cannot define new Object category as it exists\n");
				return false;
			}

			object_render_basis_class* scene_objects_of_category = new object_render_basis_class;
			if (scene_objects_of_category == NULL) {
				printf("ERROR : Cannot define new Object category : memory allocation error\n");
				return false;
			}

			scene_objects_of_category->objects_category_id = entity_category_id;
			scene_render_objects.push_back(scene_objects_of_category);
			printf("scene_entities_db_manager2_class:define_new_entity_category : %i\n", scene_render_objects.size());
			return true;
		}
		return false;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	bool define_render_object(scene_node_class <render_object_class>* entity_render_object, object_basis_class *entity_object, id_type entity_category_id) {
		int index = get_objects_of_category_index(entity_category_id);

		if (index < 0) return false;

		return scene_render_objects[index]->define_render_object(entity_object, entity_render_object);
	}

	bool define_bounding_box(scene_graph_manager_class *scene_graph_manager, id_type entity_id, object_basis_class* entity_object, id_type entity_category_id) {
		int index = get_objects_of_category_index(entity_category_id);

		if (index < 0) return false;

		return scene_render_objects[index]->define_bounding_box(scene_graph_manager, entity_id, entity_object);
	}

	bool update_shader(scene_node_class <render_object_class>* entity_render_object, id_type entity_category_id) {
		int index = get_objects_of_category_index(entity_category_id);

		if (index < 0) return false;

		return scene_render_objects[index]->update_shader(entity_render_object);
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int get_objects_of_category_index(id_type entity_category_id) {
//printf("scene_entities_db_manager2_class : get_objects_of_category_index 0000 : %i\n", scene_objects.size());
		for (int i = 0; i < scene_render_objects.size(); i++) {
//printf("scene_entities_db_manager2_class : get_objects_of_category_index 1111: %i :: %i \n", scene_objects[i]->objects_category_id, entity_category_id);
			if (scene_render_objects[i]->objects_category_id == entity_category_id)
				return i;
		}

		return -1;
	}

	void delete_entities_of_category(id_type entity_category_id) {
		if (entity_category_id > -1) {
			int index = get_objects_of_category_index(entity_category_id);
			if (index < 0) {
				// error message
				return;
			}

			scene_render_objects.erase(scene_render_objects.begin() + index);
		}
	}

	void clear_scene_entities_db() {
		scene_render_objects.clear();
	}

protected:
private:
};