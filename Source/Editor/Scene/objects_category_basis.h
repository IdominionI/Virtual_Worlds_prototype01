#pragma once

#include "../Object/object_basis.h" // *****

class scene_objects_category_class {
public:
	id_type objects_category_id = -1;

	log_panel_class* log_panel = NULL;

	virtual bool  add_new_object(id_type entity_id) { return false; }
	virtual bool  delete_object(id_type entity_id) { return false; }
	virtual object_basis_class *get_entity_object(id_type entity_id) { return NULL; }
	virtual void  delete_objects() {}

protected:

private:
};