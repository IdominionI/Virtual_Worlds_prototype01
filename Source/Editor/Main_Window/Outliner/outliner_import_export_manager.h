#pragma once

//#include <vector>

#include "../../Object/object_basis.h"

class outliner_import_export_manager_class  {
public:

	std::vector<object_import_export_basis_class*> import_export_objects;

	log_panel_class *log_panel = NULL;

	bool define_new_import_export_category(id_type import_export_object_category_id) {
		if (import_export_object_category_id > -1) {

			int index = get_import_export_category_index(import_export_object_category_id);
			if (index >= 0) {
				printf("ERROR : Cannot define new Object category as it exists\n");
				return false;
			}

			object_import_export_basis_class * import_export_of_category = new object_import_export_basis_class;
			if (import_export_of_category == NULL) {
				printf("ERROR : Cannot define new import_export category : memory allocation error\n");
				return false;
			}

			import_export_of_category->import_export_object_category_id = import_export_object_category_id;
			import_export_objects.push_back(import_export_of_category);
printf("outliner_import_emport_manager_class:define_new_import_export_category : %i\n", import_export_objects.size());
			return true;
		}
		return false;
	}

	object_import_export_basis_class *get_import_export_of_category(id_type import_export_object_category_id) {
		if (import_export_object_category_id > -1) {
			int index = get_import_export_category_index(import_export_object_category_id);
			if (index < 0) {
printf("outliner_import_emport_manager_class : get_import_export_of_category :: import_export_object_category_id == NULL\n");
				return NULL;
			}

			return import_export_objects[index];
		}

		return NULL;
	}

	int get_import_export_category_index(id_type import_export_objectcategory_id) {

//printf("outliner_import_emport_manager_class : get_import_export_category_index 0000 : %i\n", import_export_objects.size());
		for (int i = 0; i < import_export_objects.size(); i++) {
//printf("outliner_import_emport_manager_class : get_import_export_category_index 1111: %i :: %i \n", import_export_objects[i]->import_export_object_category_id, import_export_objectcategory_id);
			if (import_export_objects[i]->import_export_object_category_id == import_export_objectcategory_id)
				return i;
		}

		return -1;
	}
};