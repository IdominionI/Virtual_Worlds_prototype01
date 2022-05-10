#pragma once


#include <vector>

#include <Source/Editor/Common/definitions.h>
#include <Source/Editor/Shader/shader_import_export.h>

//#include "../../hex_surface_object/hex_surface_object.h"
//#include "../../Kernal/voxel_function_import_export.h"
//#include "../../Automata/automata_byte_import_export.h"

#include "../../Hex_surface_object/hex_surface_object.h"

class hex_surface_scene_objects_class : public std::vector<hex_surface_object_class*> {

	//voxel_function_import_export_class      voxel_function_import_export;
	//voxel_texture_import_export_class       voxel_texture_import_export;
	//import_export_byte_automata_rules_class import_export_byte_automata_rules;

public:

	hex_surface_object_class *get_hex_surface_object(object_id_type object_id){
		int index = get_object_index(object_id);
//printf("voxel_hcp_scene_objects_class :: get_voxel_hcp_object %i\n", index);
		if (index > -1) return at(index);
	
		return NULL;
	}

	//****
	bool get_shader_parameters(object_id_type object_id, shader_parameters_struct_type& shader_parameters) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		hex_surface_object_class *hex_surface_object = at(object_index);

		shader_parameters = hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters;

		return true;
	}
	//****

	bool save_generation_data(object_id_type object_id, string_type file_pathname) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		hex_surface_object_class* hex_surface_object = at(object_index);

		// TO DO
		//voxel_function_import_export.voxel_generator_parameters = hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters;

		//return voxel_function_import_export.save_voxel_generated_function(file_pathname);
	}
	/*
	bool import_generation_data(object_id_type object_id, string_type file_pathname) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		hex_surface_object_class* hex_surface_object = at(object_index);

		// TO DO
		//return voxel_function_import_export.import_voxel_genereated_function(hex_surface_object->hex_surface_object_data.voxel_generator_parameters, file_pathname);
	}
	*/
	int get_object_index(object_id_type object_id) {
//printf("voxel_hcp_scene_objects_class :: get_object_index %i \n", size());
		for (int i = 0; i < size(); i++) {
//printf("voxel_hcp_scene_objects_class :: get_object_index %i : %i\n", i, at(i)->object_id);
			if (at(i)->object_id == object_id) return i;
		}

		return -1;
	}

	bool have_object_of_id(object_id_type object_id) {
		for (int i = 0; i < size(); i++) {
			if (at(i)->object_id == object_id) return true;
		}

		return false;
	}

	bool add_new_object(object_id_type object_id) {
		hex_surface_object_class* new_object = new (hex_surface_object_class);
		if (!new_object) return false;

		new_object->object_id = object_id;
		//new_object->object_name = object_id;
		//new_hcp_object.object_category_id = ; // to be determined
		push_back(new_object);
		return true;
	}

	bool delete_object(object_id_type object_id) {
		int delete_index = get_object_index(object_id);
		if (delete_index > -1 && delete_index < size()) {

			hex_surface_object_class *voxel_hcp_object_to_delete = at(delete_index);
			delete voxel_hcp_object_to_delete;

			erase(begin()+delete_index);
			shrink_to_fit();
			return true;
		}

		return false;
	}

	bool rename_object_id(object_id_type old_object_id, object_id_type new_object_id) {
		int object_index = get_object_index(old_object_id);
		if (object_index > -1) {
			at(object_index)->object_id = new_object_id;
			return true;
		}
		return false;
	}

	// ++++++++++++++++++++++++ Voxel generation data ++++++++++++++++++++++++++++++++++++++
	//								Assign Data values
	bool define_hcp_parameter_values(object_id_type object_id, voxel_generator_parameters_struct_type data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		//QStringList data_list = data.split("|");

		//if (data_list.size() != 15) return false;

		hex_surface_object_class *hex_surface_object = at(object_index);

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname = data.expression_file_pathname;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name     = data.expression_file_name;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.x_start = data.x_start;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.x_end   = data.x_end;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.y_start = data.y_start;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.y_end   = data.y_end;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.invocation           = data.invocation;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value    = data.min_surface_value;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.max_surface_value    = data.max_surface_value;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.resolution_step      = data.resolution_step;
		//hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.generation_threshold = data.generation_threshold;

		//hex_surface_object->hex_surface_object_data.display_points = data.generator_display_data.display_as_points;
		//hex_surface_object->hex_surface_object_data.voxel_size     = data.generator_display_data.voxel_scale;

		return true;
	}

	bool define_hcp_real_var_values(object_id_type object_id, std::vector<hex_surface_generator_parameter_variable_struct_type> data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		hex_surface_object_class* hex_surface_object = at(object_index);

		if (hex_surface_object == NULL) return false;

		//QStringList var_data_list = data.split("|");

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.variables.clear();

		for (hex_surface_generator_parameter_variable_struct_type real_variable : data) {
			/*voxel_generator_parameter_variable_struct_type real_variable;
			real_variable.variable_name        = variable.variable_name;
			real_variable.value                = variable.value;
			real_variable.variable_step        = variable.variable_step;
			real_variable.active_variable      = variable.active_variable;
			real_variable.active_variable_step = variable.variable_step;*/

			hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.variables.push_back(real_variable);
		}

		return true;
	}

	bool define_hcp_int_var_values(object_id_type object_id, std::vector<hex_surface_generator_parameter_int_variable_struct_type>  data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		hex_surface_object_class* hex_surface_object = at(object_index);

		if (hex_surface_object == NULL) return false;

		//QStringList var_data_list = data.split("|");

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.int_variables.clear();

		for (hex_surface_generator_parameter_int_variable_struct_type int_variable : data) {
			/*voxel_generator_parameter_int_variable_struct_type real_variable;
			real_variable.variable_name        = variable.variable_name;
			real_variable.value                = variable.value;
			real_variable.variable_step        = variable.variable_step;
			real_variable.active_variable      = variable.active_variable;
			real_variable.active_variable_step = variable.variable_step;*/

			hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.int_variables.push_back(int_variable);
		}
		return true;
	}

	bool define_hcp_bool_var_values(object_id_type object_id, std::vector<hex_surface_generator_parameter_bool_variable_struct_type>  data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		hex_surface_object_class* hex_surface_object = at(object_index);

		if (hex_surface_object == NULL) return false;

		//QStringList var_data_list = data.split("|");

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.bool_variables.clear();

		for (hex_surface_generator_parameter_bool_variable_struct_type bool_variable : data) {
			/*voxel_generator_parameter_bool_variable_struct_type real_variable;
			real_variable.variable_name        = variable.variable_name;
			real_variable.value                = variable.value;
			real_variable.variable_step        = variable.variable_step;
			real_variable.active_variable      = variable.active_variable;
			real_variable.active_variable_step = variable.variable_step;*/

			hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.bool_variables.push_back(bool_variable);
		}

		return true;
	}

	//	++++++++++++++++++++++++ retrieve Data values ++++++++++++++++++++++++++


private:
};