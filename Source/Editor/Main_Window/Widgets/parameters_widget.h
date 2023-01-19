#pragma once

#include "../Panels/log_panel.h"
#include "../../Scene/scene_manager.h"

class parameters_widget_category_basis_class {
public:
	scene_manager_class *scene_manager = NULL;
	log_panel_class     *log_panel     = NULL;

	id_type             parameter_category_id;

	virtual void display_parameters_widget(id_type current_selected_object_id) {};

};

class parameters_widget_manager_class {
public:
	~parameters_widget_manager_class(){
		parameter_widgets.clear();
	}

	std::vector<parameters_widget_category_basis_class *> parameter_widgets;

	bool display_entity_parameters(id_type current_selected_object_id, id_type parameter_category_id) {
		int index = get_parameter_widget_of_category_index(parameter_category_id);
//printf("parameters_widget_manager_class display_entity_parameters 0000 %i \n", index);
		if (index < 0) return false;

//printf("parameters_widget_manager_class display_entity_parameters 1111 %i : %i\n", index, current_selected_object_id);
		parameter_widgets[index]->display_parameters_widget(current_selected_object_id);

		return true;
	}

	bool add_new_parameter_widget(parameters_widget_category_basis_class *parameters_widget_of_category) {
		if (parameters_widget_of_category->parameter_category_id > -1) {

			int index = get_parameter_widget_of_category_index(parameters_widget_of_category->parameter_category_id);
			if (index >= 0) {
				printf("ERROR : Cannot define new Object category as it exists\n");
				return false;
			}

			parameter_widgets.push_back(parameters_widget_of_category);
//printf("scene_entities_db_manager2_class:define_new_entity_category : %i\n", scene_objects.size());
			return true;
		}
		return false;
	}

	bool delete_parameter_category(id_type parameter_category_id) {
		if (parameter_category_id > -1 ) {
			int index = get_parameter_widget_of_category_index(parameter_category_id);
			if (index < 0) {
				// error message
				return false;
			}

			parameter_widgets.erase(parameter_widgets.begin()+index);
			return true;
		}

		return false;
	}

	int get_parameter_widget_of_category_index(id_type parameter_category_id) {

		//printf("scene_entities_db_manager2_class : get_objects_of_category_index 0000 : %i\n", scene_objects.size());
		for (int i = 0; i < parameter_widgets.size(); i++) {
//printf("scene_entities_db_manager2_class : get_objects_of_category_index 1111: %i :: %i \n", scene_objects[i]->objects_category_id, entity_category_id);
			//if (parameter_widgets[i]->parameter_category_id == parameter_category_id)
			if (parameter_widgets[i]->parameter_category_id == parameter_category_id)
				return i;
		}

		return -1;
	}

	void clear_parameter_widgets() {
		parameter_widgets.clear();
	}

};