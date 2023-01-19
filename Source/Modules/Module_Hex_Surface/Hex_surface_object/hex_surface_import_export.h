#pragma once

#include "hex_surface_object.h"

#include <Source/Editor/Tools/dialogs.h>
#include <Source/Editor/Main_Window/Panels/log_panel.h>
#include <Source/Editor/Import_Export/vw_import_export_parameters.h>

#include <Source/Editor/Object/object_basis.h> // *****

/*
	hex_surface_import_export_class

	C++ Class that handles the import and export of hex suface parameter data
	related to the generation of hex suface data from and to a formatted text
	file so as for the user to save and reuse paramaters without having to reenter the
	data from scratch.

	Method of file format to read/write this data is to flag each parameter witha <>_BLOCK_START flag
	on a line to indicate the fllowing lines up to a <>_BLOCK_END flag is a parameter. The parameter data
	is also in a particular order that dictates which line betweeen these flags corresponds to a
	particular prarmeter value.

	In a similar design, the BLOCK_START flag gives an indication that all lines up to a
	BLOCK_END flag are a set of paramater data block that have individual parameter data
	values bounded by <>_BLOCK_START and <>_BLOCK_END flags.
*/

class hex_surface_import_export_class : public object_import_export_basis_class {
public:
	std::string filename_to_write;

	std::string input_line;

	//#define endl "\n"

	void initialise(std::string _filename_to_write, log_panel_class* _log_widget) {
		filename_to_write = _filename_to_write;
		log_panel = _log_widget;

		line_number = 0;
	}

	bool import_object_data_file() {
		char const* patterns[] = { "*.hobj" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No file name defined to save data to \n Save hex surface object aborted\n");
			return false;
		}
		//else
		//	printf("export_hcp_object 00 %s \n", file_pathname);

		std::fstream import_file(file_pathname, std::ios::in);

		if (!import_file) {
			std::string str = " Could not read file \n" + (std::string)file_pathname;
			vwDialogs::message_box("Import hex surface object :", str.c_str());
			return false;
		}

		std::string object_string = FW::filetools::read_all(import_file);

		log_panel = log_panel;
		lines.clear(); lines.shrink_to_fit();
		lines = FW::stringtools::split(object_string, '\n');// Create a list of strings for each line in the expression code

		line_number = 0;
		return true;
	}

	bool export_object(object_basis_class* object, id_type category) {
		return export_hex_surface_object(dynamic_cast<hex_surface_object_class*>(object), category);
	}

	bool read_file(object_basis_class* object) {
		return read_hex_surface_object_file(dynamic_cast<hex_surface_object_class*>(object));
	}

	virtual bool export_object_data_to_file(object_basis_class* object, id_type category) {
		return export_hex_surface_object_data_to_file(dynamic_cast<hex_surface_object_class*>(object), category);
	}

	void initialise_hex_surface_export(std::string _filename_to_write, log_panel_class *_log_widget = NULL) {
		filename_to_write = _filename_to_write;
		log_panel = _log_widget;

		line_number = 0;
	}

	int open_file_stream(std::string filename) {
		filename_to_write = filename;

		stream.open(filename, std::ios::out | std::ios::trunc);// open for output (std::ios::out) and overwrite (std::ios::trunc)

		if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!

		return true;
	}

	void close_file_stream() {
		stream.close();
	}

	bool export_hex_surface_object(hex_surface_object_class *hex_surface_object, id_type category) {
		if (  hex_surface_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Unable to export hex surface object: Hex Surface Object is undefined\n");
			return false;
		}

//QMessageBox::information(NULL, "", file_pathname, QMessageBox::Ok);

		bool hex_surface_object_saved_success = true;

		char const* patterns[] = { "*.hobj" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No file name defined to save data to \n Save hex surface object aborted\n");
			return false;
		}
		//		else
		//			printf("export_hcp_object 00 %s \n", file_pathname);

		if (!open_file_stream(file_pathname)) {
			std::string str = "export voxel object ERROR:\n Failed to open file \n" + (std::string)file_pathname + "\nto write to.";
			vwDialogs::message_box("export_node", str.c_str());
			return false;
		}

		hex_surface_object_saved_success = export_hex_surface_object_data_to_file(hex_surface_object, category);

		close_file_stream();

		return hex_surface_object_saved_success;
	}

	// IMPORT FUNCTIONS

	// EXPORT FUNCTIONS

	bool export_hex_surface_object_data_to_file(hex_surface_object_class *  hex_surface_object, id_type category) {
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in export_hex_surface_object_data_to_file ()");
//}

		stream << OBJECT_BLOCK_START << endl;

		if(!save_hex_surface_general_data(hex_surface_object, category))  return false;
		if(!export_hex_surface_render_paramters(  hex_surface_object))             return false;

		if(!export_hex_surface_generator_parameters(  hex_surface_object)) return false;
		if(!export_hex_surface_texture_parameters(  hex_surface_object))   return false;
		if(!export_hex_surface_automata_parameters(  hex_surface_object))  return false;

		stream << OBJECT_BLOCK_END << endl;

		return true;
	}
private:
	bool save_hex_surface_general_data(hex_surface_object_class *hex_surface_object, id_type category) {
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in save_voxel_general_data()");
//}

		stream << DATA_BLOCK_START << endl;

		stream << hex_surface_object->object_name          << endl;
		stream << hex_surface_object->object_description   << endl;
		stream << category                                 << endl;

		stream << DATA_BLOCK_END << endl;

		return true;
	}

	// following sam as voxel_function_import_export_class save_working_generated_function() in voxel_function_import_export.h
	bool export_hex_surface_generator_parameters(  hex_surface_object_class *  hex_surface_object) {
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in export_hex_surface_generator_parameters()");
//}

		hex_surface_generator_parameters_struct_type hex_surface_generator_parameters =   hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters;
		stream << DATA_BLOCK_START << endl;

		stream << hex_surface_generator_parameters.expression_file_pathname << endl;
		stream << hex_surface_generator_parameters.expression_file_name << endl;

		stream << hex_surface_generator_parameters.x_start << endl;
		stream << hex_surface_generator_parameters.x_end   << endl;
		stream << hex_surface_generator_parameters.y_start << endl;
		stream << hex_surface_generator_parameters.y_end   << endl;

		stream << hex_surface_generator_parameters.invocation << endl;

		stream << hex_surface_generator_parameters.resolution_step      << endl;

		stream << hex_surface_generator_parameters.min_surface_value << endl;
		stream << hex_surface_generator_parameters.max_surface_value << endl;

		stream << FLOAT_VARIABLE_BLOCK_START << endl;

		for (hex_surface_generator_parameter_variable_struct_type variable : hex_surface_generator_parameters.variables) {
			stream << variable.active_variable      << endl;
			stream << variable.variable_name        << endl;
			stream << variable.value                << endl;
			stream << variable.variable_step        << endl;
			stream << variable.active_variable_step << endl;
		}

		stream << FLOAT_VARIABLE_BLOCK_END << endl;

		stream << INT_VARIABLE_BLOCK_START << endl;

		for (hex_surface_generator_parameter_int_variable_struct_type variable : hex_surface_generator_parameters.int_variables) {
			stream << variable.active_variable      << endl;
			stream << variable.variable_name        << endl;
			stream << variable.value                << endl;
			stream << variable.variable_step        << endl;
			stream << variable.active_variable_step << endl;
		}

		stream << INT_VARIABLE_BLOCK_END << endl;

		stream << BOOL_VARIABLE_BLOCK_START << endl;

		for (hex_surface_generator_parameter_bool_variable_struct_type variable : hex_surface_generator_parameters.bool_variables) {
			stream << variable.active_variable      << endl;
			stream << variable.variable_name        << endl;
			stream << variable.value                << endl;
		}

		stream << BOOL_VARIABLE_BLOCK_END << endl;

		stream << DATA_BLOCK_END << endl;

		return true;
	}

	// following sam as voxel_texture_import_export_class save_working_generated_function() in voxel_function_import_export.h
	bool export_hex_surface_texture_parameters(  hex_surface_object_class *  hex_surface_object) {
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in export_hex_surface_texture_parameters()");
//}

		material_struct_type	shader_parameters =   hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters;

		stream << DATA_BLOCK_START << endl;
		stream << FLOAT_VARIABLE_BLOCK_START << endl;
		for (shader_parameter_variable_struct_type variable : shader_parameters.variables) {
			stream << variable.active_variable      << endl;
			stream << variable.variable_name        << endl;
			stream << variable.value                << endl;
			stream << variable.variable_step        << endl;
			stream << variable.active_variable_step << endl;
			stream << variable.slider_min           << endl;
			stream << variable.slider_max           << endl;

			stream << variable.pass_to_geometry_shader << endl;
			stream << variable.pass_to_point_geometry_shader << endl;
			stream << variable.pass_to_fragment_shader       << endl;
		}
		stream << FLOAT_VARIABLE_BLOCK_END << endl;

		stream << TEXTURE_INT_BLOCK_START << endl;

		for (shader_parameter_int_variable_struct_type variable : shader_parameters.int_variables) {
			stream << variable.active_variable      << endl;
			stream << variable.variable_name        << endl;
			stream << variable.value                << endl;
			stream << variable.variable_step        << endl;
			stream << variable.active_variable_step << endl;
			stream << variable.slider_min           << endl;
			stream << variable.slider_max           << endl;

			stream << variable.pass_to_geometry_shader << endl;
			stream << variable.pass_to_point_geometry_shader << endl;
			stream << variable.pass_to_fragment_shader       << endl;
		}

		stream << TEXTURE_INT_BLOCK_END << endl;

		stream << TEXTURE_BOOL_BLOCK_START << endl;

		for (shader_parameter_bool_variable_struct_type variable : shader_parameters.bool_variables) {
			stream << variable.active_variable << endl;
			stream << variable.variable_name   << endl;
			stream << variable.value           << endl;

			stream << variable.pass_to_geometry_shader << endl;
			stream << variable.pass_to_point_geometry_shader << endl;
			stream << variable.pass_to_fragment_shader       << endl;
		}

		stream << TEXTURE_BOOL_BLOCK_END << endl;

		stream << DATA_BLOCK_END << endl;

		return true;
	}

	// following same as import_export_hex_surface_automata_rules_class write_hcp_automata_byte_rules() in automata_hex_surface_import_export.h
	bool export_hex_surface_automata_parameters(hex_surface_object_class *hex_surface_object) {
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in export_hex_surface_automata_parameters()");
//}

		stream << DATA_BLOCK_START << endl;

		for (hex_surface_automata_rule_struct_type byte_automata_rule : hex_surface_object->hex_surface_object_data.hex_surface_automata_rules) {
			stream << AUTOMATA_RULE_START << endl;

			stream << byte_automata_rule.name        << endl;
			stream << byte_automata_rule.active_rule << endl;
			stream << byte_automata_rule.hex_state << endl;
			stream << byte_automata_rule.start_step  << endl;
			stream << byte_automata_rule.end_step    << endl;

			stream << byte_automata_rule.hex_self_rule_condition.lower_rule_value << AUTOMATA_RULE_DELIMINATOR;
			switch (byte_automata_rule.hex_self_rule_condition.lower_rule) {
				case hex_lower_rule_condition_enum_type::ignore : stream << HEX_NEIGHBOR_IGNOR_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;             break;
				case hex_lower_rule_condition_enum_type::LT     : stream << HEX_NEIGHBOR_LESS_THAN_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;         break;
				case hex_lower_rule_condition_enum_type::LTEQ   : stream << HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;break;
			}

			switch (byte_automata_rule.hex_self_rule_condition.upper_rule) {
				case hex_upper_rule_condition_enum_type::ignore : stream << HEX_NEIGHBOR_IGNOR_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;             break;
				case hex_upper_rule_condition_enum_type::NE     : stream << HEX_NEIGHBOR_NOT_EQUAL_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;         break;
				case hex_upper_rule_condition_enum_type::EQ     : stream << HEX_NEIGHBOR_EQUAL_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;             break;
				case hex_upper_rule_condition_enum_type::LT     : stream << HEX_NEIGHBOR_LESS_THAN_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;         break;
				case hex_upper_rule_condition_enum_type::LTEQ   : stream << HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;break;
			}
			stream << byte_automata_rule.hex_self_rule_condition.upper_rule_value << endl;

			for(int i=0;i<NUMBER_HEX_NEIGHBOURS;i++) {
				stream << byte_automata_rule.neighbours_rule_condition[i].lower_rule_value << AUTOMATA_RULE_DELIMINATOR;
				switch (byte_automata_rule.neighbours_rule_condition[i].lower_rule) {
					case hex_lower_rule_condition_enum_type::ignore : stream << HEX_NEIGHBOR_IGNOR_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;             break;
					case hex_lower_rule_condition_enum_type::LT     : stream << HEX_NEIGHBOR_LESS_THAN_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;         break;
					case hex_lower_rule_condition_enum_type::LTEQ   : stream << HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;break;
				}

				switch (byte_automata_rule.neighbours_rule_condition[i].upper_rule) {
					case hex_upper_rule_condition_enum_type::ignore : stream << HEX_NEIGHBOR_IGNOR_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;             break;
					case hex_upper_rule_condition_enum_type::NE     : stream << HEX_NEIGHBOR_NOT_EQUAL_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;         break;
					case hex_upper_rule_condition_enum_type::EQ     : stream << HEX_NEIGHBOR_EQUAL_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;             break;
					case hex_upper_rule_condition_enum_type::LT     : stream << HEX_NEIGHBOR_LESS_THAN_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;         break;
					case hex_upper_rule_condition_enum_type::LTEQ   : stream << HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << AUTOMATA_RULE_DELIMINATOR;break;
				}
				stream << byte_automata_rule.neighbours_rule_condition[i].upper_rule_value << endl;
			}

			stream << AUTOMATA_RULE_END << endl;
		}

		stream << DATA_BLOCK_END << endl;

		return true;
	}

	bool export_hex_surface_render_paramters(hex_surface_object_class *hex_surface_object) {
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in export_hex_surface_render_paramters()");
//}
		if (hex_surface_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Unable to export hex surface object rendering shader data for object %s hex surface object undefined.\n", hex_surface_object->object_name.c_str());
			return false;
		}

		stream << DATA_BLOCK_START << endl;

		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.default_vertex_shader_file_pathname.string() << endl;
		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.vertex_shader_file_pathname.string() << endl;
		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.default_point_shader_file_pathname.string() << endl;
		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.point_shader_file_pathname.string() << endl;
		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.default_fragment_shader_file_pathname.string() << endl;
		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.fragment_shader_file_pathname.string() << endl;

		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.vertex_shader_file_pathname.string() << endl;

		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.point_shader_file_pathname.string() << endl;

		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.fragment_shader_file_pathname.string() << endl;

		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.use_default_vertex_shader << endl;
		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.use_default_point_shader << endl;
		stream << hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.use_default_fragment_shader << endl;

		stream << DATA_BLOCK_END << endl;

		return true;
	}

	// IMPORT FUNCTIONS
public:
	bool read_hex_surface_object_file(hex_surface_object_class *hex_surface_object) {
//QMessageBox::information(NULL, "Import voxel object", "read_voxel_object_file 00  " + std::string::number(line_number), QMessageBox::Ok);
//if (log_widget != NULL ) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in read_voxel_object_file 00");
//}

//QMessageBox::information(NULL, "Import voxel object", "read_voxel_object_file 02  " + std::string::number(line_number), QMessageBox::Ok);
		if (hex_surface_object == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Hex surface object to import data into is undefined\n");
			vwDialogs::message_box("Import Hex surface Object", "Hex surface object to import data into is undefined");
			return false;
		}

//QMessageBox::information(NULL, "Import voxel object", "read_voxel_object_file 03  " + std::string::number(line_number), QMessageBox::Ok);
		// Find the line or index number in the list of lines of the expression that has the expression variable list defined 
		while (!(FW::stringtools::contains(lines[line_number], OBJECT_BLOCK_START)))
			line_number++;

		// If have not found a defined expression return an error
		if (line_number >= lines.size()) {
//QMessageBox::information(NULL, "Import voxel object", "Import voxel generation model ERROR : \n Could not find start block to import parameter data.", QMessageBox::Ok);
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object start block to import parameter data.\n");
			return false;
		}

//QMessageBox::information(NULL, "Import voxel object", "read_voxel_object_file 04  " + std::string::number(line_number), QMessageBox::Ok);
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in read_voxel_object_file 01");
//}

		id_type category;

		if (!define_hex_surface_general_data(hex_surface_object,category)){
			delete    hex_surface_object;
			return false;}

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in read_voxel_object_file 01A");
//}

		// !!!!!!!!!!!!!!!!!!!!! FOR FUTURE MODIFICATION AND UPGRADE !!!!!!!!!!!!!!!!!!!!!!!!!!!!

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in read_voxel_object_file 01B");
//}
		if (!define_hex_surface_render_paramters(  hex_surface_object)) {
			delete    hex_surface_object;
			return false;
		}
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "hcp_voxel_import_export_class::read_voxel_object_file 01C");
//}
		if (!define_hex_surface_generator_parameters(  hex_surface_object)) {
			delete    hex_surface_object;
			return false;
		}
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "hcp_voxel_import_export_class::read_voxel_object_file 01D");
//}
		if (!define_hex_surface_texture_parameters(  hex_surface_object)) {
			delete    hex_surface_object;
			return false;
		}
//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "hcp_voxel_import_export_class::read_voxel_object_file 01E");
//}
		
		if (!define_hex_surface_automata_parameters(  hex_surface_object)) {
			delete    hex_surface_object;
			return false;
		}

//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "hcp_voxel_import_export_class::read_voxel_object_file 02");
//}
//QMessageBox::information(NULL, "Import voxel object", "read_voxel_object_file 05  " + std::string::number(line_number), QMessageBox::Ok);

//if (log_widget != NULL && line_number < lines.size()) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "object_group->group_objects 00: " + object_group->group_objects[object_group->group_objects.size()-1].object_data_type_ptr.  hex_surface_object->object_name);
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "object_group->group_objects 01: " + std::string::number(line_number) + " : " + lines[line_number]);
//}

		line_number++;
		if (!(FW::stringtools::contains(lines[line_number], OBJECT_BLOCK_END))) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object end block to import parameter data.\n");
			return false;
		}

		return true;
	}

private:
	bool define_hex_surface_general_data(hex_surface_object_class *hex_surface_object, id_type &category) {
		line_number++;
		if (lines.size() < line_number + 3) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file  hex seruface general data\n");
			return false;
		}

//if (log_widget != NULL && line_number < lines.size()) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in define_voxel_general_data 00 : " + std::string::number(line_number) + " : " + lines[line_number]);
//}

		if (!(FW::stringtools::contains(lines[line_number], DATA_BLOCK_START))) return false;

		line_number++;

		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size()); // get rid of end of line char
		hex_surface_object->object_name = line; line_number++;

		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size()); // get rid of end of line char
		hex_surface_object->object_description = line; line_number++;

		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size()); // get rid of end of line char
		category = std::stoi(line); line_number++;


//if (log_widget != NULL) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in define_voxel_general_data 01 : " + std::string::number(line_number) + " : " + lines[line_number]);
//}

		if (!(FW::stringtools::contains(lines[line_number], DATA_BLOCK_END))) return false;

		return true;
	}

	bool define_hex_surface_render_paramters(  hex_surface_object_class *hex_surface_object) {
		line_number++;
		if (lines.size() < line_number + 17) {
			//vwDialogs::message_box("Import voxel generation model", "Import voxel generation model ERROR : \n missing parameter data for filename and matrix definitions.");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Hex surface render parameters\n");
			return false;
		}

//if (log_widget != NULL && line_number < lines.size()) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in define_hex_surface_render_paramters 00 : " + std::string::number(line_number) + " : " + lines[line_number]);
//}

		if (!(FW::stringtools::contains(lines[line_number], DATA_BLOCK_START))) return false;

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.default_vertex_shader_file_pathname = line;

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.vertex_shader_file_pathname = line;

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.default_point_shader_file_pathname = line;

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.point_shader_file_pathname = line;

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.default_fragment_shader_file_pathname = line;

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.fragment_shader_file_pathname = line;

//printf( "hcp_voxel_import_export_class::define_voxel_render_paramters 2222 : %i \n", line_number);
		// -------------------------------------------------------------------------------

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.vertex_shader_file_pathname = line;

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.point_shader_file_pathname = line;

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.fragment_shader_file_pathname = line;

//printf( "hcp_voxel_import_export_class::define_voxel_render_paramters 3333 : %i \n", line_number);
		// ------------------------- 

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.use_default_vertex_shader = stoi(line);

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.use_default_point_shader = stoi(line);

		line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.use_default_fragment_shader = stoi(line);


//if (log_widget != NULL && line_number < lines.size()) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in define_hex_surface_render_paramters 01 : " + std::string::number(line_number) + " : " + lines[line_number]);
//}

		line_number++;
//printf( "hcp_voxel_import_export_class::define_voxel_render_paramters 5555 : %i : %s\n", line_number, lines[line_number].c_str());
		if (!(FW::stringtools::contains(lines[line_number], DATA_BLOCK_END))) return false;

		return true;
	}

	bool define_hex_surface_generator_parameters(  hex_surface_object_class *  hex_surface_object) {
		line_number++;
		if (lines.size() < line_number + 15) {
			//vwDialogs::message_box("Import voxel generation model", "Import voxel generation model ERROR : \n missing parameter data for filename and matrix definitions.");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : hex surface generator paramteters\n");
			return false;
		}

//if (log_widget != NULL && line_number < lines.size()) {
//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "in define_hex_surface_generator_parameters 00 : " + std::string::number(line_number) + " : " + lines[line_number]);
//}
		if (!FW::stringtools::contains(lines[line_number], DATA_BLOCK_START)) return false;

		line_number++;
		std::string line = lines[line_number];  line = FW::stringtools::truncate(line, line.size()); // get rid of end of line char
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname = line; line_number++;

		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name     = line; line_number++;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.x_start = std::stof(lines[line_number]); line_number++;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.x_end   = std::stof(lines[line_number]); line_number++;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.y_start = std::stof(lines[line_number]); line_number++;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.y_end   = std::stof(lines[line_number]); line_number++;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.invocation = std::stoi(lines[line_number]); line_number++;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.resolution_step      = std::stof(lines[line_number]); line_number++;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value = std::stof(lines[line_number]); line_number++;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.max_surface_value = std::stof(lines[line_number]); line_number++;

		if (!FW::stringtools::contains(lines[line_number], FLOAT_VARIABLE_BLOCK_START)) {
			vwDialogs::message_box("Import voxel generation model", "Import voxel generation model ERROR : \n Could not find start of float variable data to import float variables");
			return false;
		}


		line_number++;
		while (!FW::stringtools::contains(lines[line_number], FLOAT_VARIABLE_BLOCK_END)) {
			if (line_number + 4 > lines.size() ) {
				vwDialogs::message_box("Import hex surface generation model", "Import voxel generation model ERROR : \n Have function float variable data incorrectly defined.");
//vwDialogs::message_box("Import voxel generation model", "Here 002 : " + std::string::number(line_number));
				return false;
			}

			hex_surface_generator_parameter_variable_struct_type variable;
			variable.active_variable      = std::stoi(lines[line_number]); line_number++;
			line = lines[line_number];	line = FW::stringtools::truncate(line, line.size());
			variable.variable_name        = line; line_number++;
			variable.value                = std::stof(lines[line_number]); line_number++;
			variable.variable_step        = std::stof(lines[line_number]); line_number++;
			variable.active_variable_step = std::stoi(lines[line_number]); line_number++;
//QMessageBox::information(NULL, "Import voxel generation model", "Here 003 : " + std::string::number(line_number), QMessageBox::Ok);
			  hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.variables.push_back(variable);
//QMessageBox::information(NULL, "Import voxel generation model", "Here 004 : " + lines[line_number], QMessageBox::Ok);
		}


		line_number++;
		if (!FW::stringtools::contains(lines[line_number], INT_VARIABLE_BLOCK_START)) {
			vwDialogs::message_box("Import hex surface generation model", "Import vox hex surfaceel generation model ERROR : \n Could not find start of integer variable data to import integer variables");
			return false;
		}

		line_number++;
		while (!FW::stringtools::contains(lines[line_number], INT_VARIABLE_BLOCK_END)) {
			if (line_number + 4 > lines.size()) {
				vwDialogs::message_box("Import  hex surface generation model", "Import vo hex surfacexel generation model ERROR : \n Have function integer variable data incorrectly defined.");
//vwDialogs::message_box("Import voxel generation model", "Here 002 : " + std::string::number(line_number));
				return false;
			}


			hex_surface_generator_parameter_int_variable_struct_type int_variable;
			int_variable.active_variable      = std::stoi(lines[line_number]); line_number++;
			line =  lines[line_number];  line = FW::stringtools::truncate(line, line.size());
			int_variable.variable_name        = line; line_number++;
			int_variable.value                = std::stoi(lines[line_number]); line_number++;
			int_variable.variable_step        = std::stoi(lines[line_number]); line_number++;
			int_variable.active_variable_step = std::stoi(lines[line_number]); line_number++;
//QMessageBox::information(NULL, "Import voxel generation model", "Here 003 : " + std::string::number(line_number), QMessageBox::Ok);
			  hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.int_variables.push_back(int_variable);
//QMessageBox::information(NULL, "Import voxel generation model", "Here 004 : " + lines[line_number], QMessageBox::Ok);
		}

		line_number++;
		if (!FW::stringtools::contains(lines[line_number], BOOL_VARIABLE_BLOCK_START)) {
			vwDialogs::message_box("Import hex surface generation model", "Import hex surface generation model ERROR : \n Could not find start of integer variable data to import integer variables");
			return false;
		}

		line_number++;
		while (!FW::stringtools::contains(lines[line_number], BOOL_VARIABLE_BLOCK_END)) {
			if (line_number + 2 > lines.size()) {
				vwDialogs::message_box("Import hex surface generation model", "Import hex surface generation model ERROR : \n Have function integer variable data incorrectly defined.");
//vwDialogs::message_box("Import voxel generation model", "Here 002 : " + std::string::number(line_number));
				return false;
			}

			hex_surface_generator_parameter_bool_variable_struct_type bool_variable;
			bool_variable.active_variable      = std::stoi(lines[line_number]); line_number++;
			line =  lines[line_number];   line = FW::stringtools::truncate(line, line.size());
			bool_variable.variable_name        = line; line_number++;
			bool_variable.value                = std::stoi(lines[line_number]); line_number++;
//QMessageBox::information(NULL, "Import voxel generation model", "Here 003 : " + std::string::number(line_number), QMessageBox::Ok);
		    hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.bool_variables.push_back(bool_variable);
//QMessageBox::information(NULL, "Import voxel generation model", "Here 004 : " + lines[line_number], QMessageBox::Ok);
		}

		line_number++;
		if (!FW::stringtools::contains(lines[line_number], DATA_BLOCK_END)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. :  Could not find end data block flag after loading parameter file.\n");
		}

		return true;

	}

	bool define_hex_surface_texture_parameters(hex_surface_object_class *hex_surface_object) {
		line_number++;

//vwDialogs::message_box("Import voxel generation model", "Here 00: " + std::string::number(line_number) + ":"+ lines[line_number]);
		if (!FW::stringtools::contains(lines[line_number], DATA_BLOCK_START)) {
			if (log_panel != NULL) {
				log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file :  Could not find start texture block to import integer parameter data.\n");
				log_panel->application_log.AddLog("DEBUG : Object data file is incorrectly defined 00 : %i : %s.\n", line_number, lines[line_number]);
			}

			return false;
		}

		line_number++;

		if (!FW::stringtools::contains(lines[line_number], FLOAT_VARIABLE_BLOCK_START)) {
			log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file :  Could not find start loat block to import integer parameter data.\n");
			return false;
		}
		line_number++;
//vwDialogs::message_box("Import voxel generation model", "Here 001: " + std::string::number(line_number));

		while (!FW::stringtools::contains(lines[line_number], FLOAT_VARIABLE_BLOCK_END)) {
			if (line_number + 9 > lines.size() ) {
//vwDialogs::message_box("Import voxel texture model", "Import voxel texture model ERROR : \n Have function variable data incorrectly defined.");
				log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : voxel texture variable parameters not defined correctly\n");
				return false;
			}

			shader_parameter_variable_struct_type variable;
			variable.active_variable      = std::stoi(lines[line_number]); line_number++;
			line =  lines[line_number];   line = FW::stringtools::truncate(line, line.size());
			variable.variable_name        = line; line_number++;
			variable.value                = std::stof(lines[line_number]); line_number++;
			variable.variable_step        = std::stof(lines[line_number]); line_number++;
			variable.active_variable_step = std::stof(lines[line_number]);   line_number++;
			variable.slider_min           = std::stof(lines[line_number]);   line_number++;
			variable.slider_max           = std::stof(lines[line_number]);   line_number++;

			// ****
			variable.pass_to_geometry_shader       = std::stoi(lines[line_number]); line_number++;
			variable.pass_to_point_geometry_shader = std::stoi(lines[line_number]); line_number++;
			variable.pass_to_fragment_shader       = std::stoi(lines[line_number]); line_number++;
			// ****

//vwDialogs::message_box("Import voxel generation model", "Here 003 : " + std::string::number(line_number));
			hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.variables.push_back(variable);
//vwDialogs::message_box("Import voxel generation model", "Here 004 : " + lines[line_number]);
		}

		// Find the line or index number in the list of lines of the expression that has the expression integer variable list defined 
		while (!FW::stringtools::contains(lines[line_number], (TEXTURE_INT_BLOCK_START)))
			line_number++;

		// If have not found a defined expression return an error
		if (line_number >= lines.size()) {
//vwDialogs::message_box("Import voxel generation model", "Import virtual painter generation model ERROR : \n Could not find start block to import integer parameter data.");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file :  Could not find start block to import integer parameter data.\n");
			return false;
		}

		if (!FW::stringtools::contains(lines[line_number], TEXTURE_INT_BLOCK_START)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file :  Could not find start integer block to import integer parameter data.\n");
			return false;
		}
		line_number++;

//vwDialogs::message_box("Import voxel generation model", "Here 001: " + std::string::number(line_number));

		while (!FW::stringtools::contains(lines[line_number], TEXTURE_INT_BLOCK_END)) {
			if (line_number + 9 > lines.size() ) {
//vwDialogs::message_box("Import voxel texture model", "Import virtual painter parameter ERROR : \n Have integer variable data incorrectly defined.");
//vwDialogs::message_box("Import voxel generation model", "Here 002 : " + std::string::number(line_number));
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Have integer variable data incorrectly defined.\n");
				return false;
			}

			shader_parameter_int_variable_struct_type variable;
			variable.active_variable      = std::stoi(lines[line_number]); line_number++;
			line =  lines[line_number];   line = FW::stringtools::truncate(line, line.size());
			variable.variable_name        = line; line_number++;
			variable.value                = std::stoi(lines[line_number]); line_number++;
			variable.variable_step        = std::stoi(lines[line_number]); line_number++;
			variable.active_variable_step = std::stoi(lines[line_number]); line_number++;
			variable.slider_min           = std::stoi(lines[line_number]); line_number++;
			variable.slider_max           = std::stoi(lines[line_number]); line_number++;

			// ****
			variable.pass_to_geometry_shader = std::stoi(lines[line_number]); line_number++;
			variable.pass_to_point_geometry_shader = std::stoi(lines[line_number]); line_number++;
			variable.pass_to_fragment_shader       = std::stoi(lines[line_number]); line_number++;
			// ****

//vwDialogs::message_box("Import voxel generation model", "Here 003 : " + std::string::number(line_number));
			hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.int_variables.push_back(variable);
//vwDialogs::message_box("Import voxel generation model", "Here 004 : " + lines[line_number]);
		}


		// Find the line or index number in the list of lines of the expression that has the expression boolean variable list defined 
		while (!FW::stringtools::contains(lines[line_number], TEXTURE_BOOL_BLOCK_START))
			line_number++;

		// If have not found a defined expression return an error
		if (line_number >= lines.size()) {
			//vwDialogs::message_box("Import voxel generation model", "Import virtual painter generation model ERROR : \n Could not find start block to import boolean parameter data.");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Could not find start block to import boolean parameter data.\n");
			return false;
		}

		if (!FW::stringtools::contains(lines[line_number], TEXTURE_BOOL_BLOCK_START)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file :  Could not find start boolean block to import integer parameter data.\n");
			return false;
		}
		line_number++;

//vwDialogs::message_box("Import voxel generation model", "Here 001: " + std::string::number(line_number));

		while (!FW::stringtools::contains(lines[line_number], TEXTURE_BOOL_BLOCK_END)) {
			if (line_number + 5 > lines.size() ) {
				//vwDialogs::message_box("Import voxel texture model", "Import virtual painter parameter ERROR : \n Have boolean variable data incorrectly defined.");
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Have boolean variable data incorrectly defined.\n");
//vwDialogs::message_box("Import voxel generation model", "Here 002 : " + std::string::number(line_number));
				return false;
			}

			shader_parameter_bool_variable_struct_type variable;
			variable.active_variable      = std::stoi(lines[line_number]); line_number++;
			line =  lines[line_number];   line = FW::stringtools::truncate(line, line.size());
			variable.variable_name        = line; line_number++;
			variable.value                = std::stoi(lines[line_number]); line_number++;

			// ****
			variable.pass_to_geometry_shader = std::stoi(lines[line_number]); line_number++;
			variable.pass_to_point_geometry_shader = std::stoi(lines[line_number]); line_number++;
			variable.pass_to_fragment_shader       = std::stoi(lines[line_number]); line_number++;
			// ****

//vwDialogs::message_box("Import voxel generation model", "Here 003 : " + std::string::number(line_number));
			hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters.bool_variables.push_back(variable);
//vwDialogs::message_box("Import voxel generation model", "Here 004 : " + lines[line_number]);
		}

		line_number++;

		//if (!lines[line_number].contains(DATA_BLOCK_END)) {
		if (!FW::stringtools::contains(lines[line_number], DATA_BLOCK_END)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Have no end of data block defind for voxel object texture data.\n");
			return false;
		}

		return true;
	}

	bool define_hex_surface_automata_parameters(hex_surface_object_class *hex_surface_object) {
		line_number++;

		if (!FW::stringtools::contains(lines[line_number], DATA_BLOCK_START)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Have no start of data block defind for voxel object automata data.\n");
			return false;
		}

		line_number++;
		if (FW::stringtools::contains(lines[line_number], DATA_BLOCK_END)) { // Have no Aautoamta data to import
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Have no automata rule data to import.\n");
			return true;
		}

		if (!FW::stringtools::contains(lines[line_number], AUTOMATA_RULE_START)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Have no start of automata rule data defind for voxel object.\n");
			return false;
		}
		
		line_number++;
		if (FW::stringtools::contains(lines[line_number], AUTOMATA_RULE_END)) { // No autoamata rule data to import
			line_number++;
			if (FW::stringtools::contains(lines[line_number], DATA_BLOCK_END))
				return true;// No autoamata rule data to import
			else {
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Have no end of data block defind for voxel object automata data.\n");
			}

		}

		// !!!!!!!!!! automata data on to do list !!!!!!!!!!!!!
		// Have automata rule data to import
		while (!FW::stringtools::contains(lines[line_number], DATA_BLOCK_END) && line_number < lines.size()) {
			hex_surface_automata_rule_struct_type hex_surface_automata_rule;
			if (!read_automata_byte_rule(hex_surface_automata_rule)) return false;

			hex_surface_object->hex_surface_object_data.hex_surface_automata_rules.push_back(hex_surface_automata_rule);
			line_number++;
		}

		if (line_number >= lines.size() || !FW::stringtools::contains(lines[line_number], DATA_BLOCK_END)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Premature end of data reached or voxel automata data inorrectly defined. No end of autmata rules data flag present.\n");
			return false;
		}

		return true;
	}

	bool read_automata_byte_rule(hex_surface_automata_rule_struct_type &hex_surface_automata_rule) {
		std::string line;

		if (FW::stringtools::contains(lines[line_number], AUTOMATA_RULE_START)) line_number++;

		if (lines.size() < line_number + 12) {
//vwDialogs::message_box("Import voxel generation model", "Import voxel generation model ERROR : \n missing parameter data for filename and matrix definitions.");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Hex surface automata rule is not defined correctly.\n");
			return false;
		}

		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		hex_surface_automata_rule.name        = line; line_number++;
		hex_surface_automata_rule.active_rule = std::stoi(lines[line_number]); line_number++;
		hex_surface_automata_rule.hex_state   = std::stof(lines[line_number]); line_number++;
		hex_surface_automata_rule.start_step  = std::stoi(lines[line_number]); line_number++;
		hex_surface_automata_rule.end_step    = std::stoi(lines[line_number]); line_number++;

//QMessageBox::information(NULL,"Import Automata byte rules", "read_automata_byte_rule 11 " + std::string::number(line_number) + " : " + lines[line_number], QMessageBox::Ok);
		// next line contains the self rule definitions where each rule defenintion is seperated by the deliminator RULE_DELIMINATOR
		std::vector<std::string> rule_list = FW::stringtools::split(lines[line_number], AUTOMATA_RULE_DELIMINATOR);

		if(rule_list.size()<4){
//QMessageBox::information(NULL,"Import Automata byte rules", "Error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n"+"Incorrect parameters for self rule", QMessageBox::Ok);
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Error at line %i of imported file Incorrect parameters for self rule\n", line_number);
			return false;
		}

		hex_surface_automata_rule.hex_self_rule_condition.lower_rule_value = std::stof(rule_list[0]);
		switch (std::stoi(rule_list[1])) {
			case HEX_NEIGHBOR_IGNOR_RULE_CODE              : hex_surface_automata_rule.hex_self_rule_condition.lower_rule = hex_lower_rule_condition_enum_type::ignore; break;
			case HEX_NEIGHBOR_LESS_THAN_RULE_CODE          : hex_surface_automata_rule.hex_self_rule_condition.lower_rule = hex_lower_rule_condition_enum_type::LT;     break;
			case HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE : hex_surface_automata_rule.hex_self_rule_condition.lower_rule = hex_lower_rule_condition_enum_type::LTEQ;   break;
		}

		switch (std::stoi(rule_list[2])) {
			case HEX_NEIGHBOR_IGNOR_RULE_CODE              : hex_surface_automata_rule.hex_self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::ignore; break;
			case HEX_NEIGHBOR_NOT_EQUAL_RULE_CODE          : hex_surface_automata_rule.hex_self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::NE;     break;
			case HEX_NEIGHBOR_EQUAL_RULE_CODE              : hex_surface_automata_rule.hex_self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::EQ;     break;
			case HEX_NEIGHBOR_LESS_THAN_RULE_CODE          : hex_surface_automata_rule.hex_self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::LT;     break;
			case HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE : hex_surface_automata_rule.hex_self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::LTEQ;   break;
		}
		hex_surface_automata_rule.hex_self_rule_condition.upper_rule_value = std::stof(rule_list[3]);

		// next NUMBER_HEX_NEIGHBOURS lines must have the neighbour activity states for the rule
		for (int i = 0; i < NUMBER_HEX_NEIGHBOURS; i++){
			line_number++;

//QMessageBox::information(NULL,"Import Automata byte rules", "read_automata_byte_rule 22 " + std::string::number(line_number) + " : " + lines[line_number], QMessageBox::Ok);
			if(line_number >= lines.size())  {
//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", QMessageBox::Ok);
				if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :  Error at line %i of imported file Incorrect parameters for self rule\n", line_number);
				return false;
			}

			rule_list = FW::stringtools::split(lines[line_number], AUTOMATA_RULE_DELIMINATOR);

			if (rule_list.size() < 4) {
				std::string str = "Error at line " + std::to_string(line_number) + " of import file. Incorrect parameters for self rule";
				vwDialogs::message_box("Import Automata byte rules", str.c_str());
				return false;
			}

			hex_surface_automata_rule.neighbours_rule_condition[i].lower_rule_value = std::stof(rule_list[0]);
			switch (std::stoi(rule_list[1])) {
				case HEX_NEIGHBOR_IGNOR_RULE_CODE              : hex_surface_automata_rule.neighbours_rule_condition[i].lower_rule = hex_lower_rule_condition_enum_type::ignore; break;
				case HEX_NEIGHBOR_LESS_THAN_RULE_CODE          : hex_surface_automata_rule.neighbours_rule_condition[i].lower_rule = hex_lower_rule_condition_enum_type::LT;     break;
				case HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE : hex_surface_automata_rule.neighbours_rule_condition[i].lower_rule = hex_lower_rule_condition_enum_type::LTEQ;   break;
			}

			switch (std::stoi(rule_list[2])) {
				case HEX_NEIGHBOR_IGNOR_RULE_CODE              : hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::ignore; break;
				case HEX_NEIGHBOR_NOT_EQUAL_RULE_CODE          : hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::NE;     break;
				case HEX_NEIGHBOR_EQUAL_RULE_CODE              : hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::EQ;     break;
				case HEX_NEIGHBOR_LESS_THAN_RULE_CODE          : hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::LT;    break;
				case HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE : hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::LTEQ;   break;
			}
			hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule_value = std::stof(rule_list[3]);
		}


		line_number++;
//QMessageBox::information(NULL,"Import Automata byte rules", "read_automata_byte_rule 33 " + std::string::number(line_number) + " : " + lines[line_number], QMessageBox::Ok);
		if (line_number >= lines.size() || !FW::stringtools::contains(lines[line_number], AUTOMATA_RULE_END)) {
			if (log_panel != NULL) {
				if(line_number >= lines.size())
					log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : Premature end of data reached.\n");
				else
					log_panel->application_log.AddLog("ERROR : Object data file is incorrectly defined. Cannot load file : No end of automata rules data flag present.\n");

			}
			return false;
		}

		return true;
	}

	std::string line;
};

