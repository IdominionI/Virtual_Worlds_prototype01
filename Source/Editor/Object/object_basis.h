#pragma once

#include "../Common/definitions.h"

/*
					OBJECT BASIS

	A basis class of common attributes, parameters etd
	that all entity objects must have and thus inherit
*/

class scene_manager_class; // Critical to include this forward declaration. 
						   //Cannot find circular declarations that cause error C2061

class object_basis_class {
public:

	object_id_type  	object_id          = 0;
	category_id_type	object_category_id = 0;
	name_type			object_name        = "object";
	description_type	object_description = "";

	bool active_object = false;

	virtual bool define_geometry() { return false; }
	virtual void update_bounding_display() {}

	virtual void clear_shader_variables() {}

	//******
	//virtual void display_parameters(scene_manager_class *scene_manager = NULL, log_panel_class *log_panel = NULL, id_type current_selected_object_id = -1) { }
	//******

	//object_basis_class() {
	//}

	//~object_basis_class() {
	//}

private:

};

#include "../Main_Window/Panels/log_panel.h" //*****

class object_import_export_basis_class {
public:
	id_type import_export_object_category_id;

	log_panel_class	*log_panel = NULL;

	int line_number = 0;
	std::vector<std::string> lines;

	std::fstream stream;

	virtual bool import_object_data_file() { return false; }

	virtual void initialise(std::string _filename_to_write, log_panel_class *log_panel = NULL) {}
	virtual bool export_object(object_basis_class *object, id_type category) { return false; }
	virtual bool read_file(object_basis_class *object) { return false; }

	virtual bool export_object_data_to_file(object_basis_class *object, id_type category) { return false; }
};