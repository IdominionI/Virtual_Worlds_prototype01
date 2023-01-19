#include "voxel_function_import_export.h"

#include <vector>

#include <Universal/Framework/FWstring_tools.h>
#include <Universal/Framework/FWfile_tools.h>

#include <Source/Graphics_Engine/Shader/shader_parameters.h>


	int voxel_function_import_export_class::open_file_stream(std::string filename, int write_mode) {
		if (write_mode == OVER_WRITE_FUNCTION_FILE) {
			stream.open(filename, std::ios::out | std::ios::trunc);// open for output (std::ios::out) and overwrite (std::ios::trunc)

			if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!
		}
		else
			if (!stream.is_open()) return false;

		return true;
	}

	void voxel_function_import_export_class::close_file_stream() {
		stream.close();
	}

	bool voxel_function_import_export_class::save_voxel_generated_function(std::string file_pathname) {
////QMessageBox::information(NULL, "", "in save_working_generated_function", //QMessageBox::Ok);

		if (file_pathname.size() == 0) {
//QMessageBox::information(NULL, "", "No file name defined to save data to \n Save Static Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) return false;

		stream << BLOCK_START << ENDL;

		stream << voxel_generator_parameters.expression_file_pathname << ENDL;
		stream << voxel_generator_parameters.expression_file_name << ENDL;

		stream << voxel_generator_parameters.x_start << ENDL;
		stream << voxel_generator_parameters.x_end   << ENDL;
		stream << voxel_generator_parameters.y_start << ENDL;
		stream << voxel_generator_parameters.y_end   << ENDL;
		stream << voxel_generator_parameters.z_start << ENDL;
		stream << voxel_generator_parameters.z_end   << ENDL;

		stream << voxel_generator_parameters.invocation << ENDL;

		stream << voxel_generator_parameters.resolution_step      << ENDL;
		stream << voxel_generator_parameters.generation_threshold << ENDL;

		stream << voxel_generator_parameters.min_surface_value << ENDL;
		stream << voxel_generator_parameters.max_surface_value << ENDL;

		stream << FLOAT_BLOCK_START << ENDL;

		for (voxel_generator_parameter_variable_struct_type variable : voxel_generator_parameters.variables) {
			stream << variable.active_variable       << ENDL;
			stream << variable.variable_name.c_str() << ENDL; // Need to export as a const char* type to avoid trailing nulls at end of std::string type
			stream << variable.value                 << ENDL;
			stream << variable.variable_step         << ENDL;
			stream << variable.active_variable_step  << ENDL;
		}

		stream << FLOAT_BLOCK_END << ENDL;

		stream << INT_BLOCK_START << ENDL;

		for (voxel_generator_parameter_int_variable_struct_type variable : voxel_generator_parameters.int_variables) {
			stream << variable.active_variable       << ENDL;
			stream << variable.variable_name.c_str() << ENDL;// Need to export as a const char* type to avoid trailing nulls at end of std::string type
			stream << variable.value                 << ENDL;
			stream << variable.variable_step         << ENDL;
			stream << variable.active_variable_step  << ENDL;
		}

		stream << INT_BLOCK_END << ENDL;

		stream << BOOL_BLOCK_START << ENDL;

		for (voxel_generator_parameter_bool_variable_struct_type variable : voxel_generator_parameters.bool_variables) {
			stream << variable.active_variable       << ENDL;
			stream << variable.variable_name.c_str() << ENDL;// Need to export as a const char* type to avoid trailing nulls at end of std::string type
			stream << variable.value                 << ENDL;
		}

		stream << BOOL_BLOCK_END << ENDL;

		stream << BLOCK_END << ENDL;

		close_file_stream();

		return true;
	}

	// -------------------------------------------------------------------

	bool voxel_function_import_export_class::import_voxel_genereated_function(voxel_generator_parameters_struct_type &generator_parameters, std::string file_pathname) {
		if (file_pathname.size() == 0) {
//QMessageBox::information(NULL, "", "No file name defined to import data from \n Import Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		std::fstream working_model_file(file_pathname, std::ios::in);

		if (!working_model_file) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find read Import voxel generation model defined in file \n" +
//																 file_pathname, //QMessageBox::Ok);
			return false;
		}

		std::string working_model_string = FW::filetools::read_all(working_model_file);

		return read_expression_into_voxel_generator_parameters(working_model_string, generator_parameters);
	}

	bool voxel_function_import_export_class::read_expression_into_voxel_generator_parameters(std::string working_model_string,voxel_generator_parameters_struct_type &generator_parameters) {
//QStringList lines = working_model_string.split('\n');// Create a list of strings for each line in the expression code
		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
		
		std::string line;
		int line_number = 0;

		// Find the line or index number in the list of lines of the expression that has the expression variable list defined 
		while (!FW::stringtools::contains(lines[line_number],BLOCK_START))
			line_number++;

		// If have not found a defined expression return an error
		if (line_number >= lines.size()) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find start block to import parameter data.", //QMessageBox::Ok);
			return false;
		}

		if (lines.size() < line_number + 13) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n missing parameter data for filename and matrix definitions.", //QMessageBox::Ok);
			return false;
		}

		line_number++;
		line =  lines[line_number];  line = FW::stringtools::truncate(line,line.size());
		generator_parameters.expression_file_pathname  = line; line_number++;

		line =  lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		generator_parameters.expression_file_name      = line; line_number++;

		generator_parameters.x_start = stof(lines[line_number]); line_number++;
		generator_parameters.x_end   = stof(lines[line_number]); line_number++;
		generator_parameters.y_start = stof(lines[line_number]); line_number++;
		generator_parameters.y_end   = stof(lines[line_number]); line_number++;
		generator_parameters.z_start = stof(lines[line_number]); line_number++;
		generator_parameters.z_end   = stof(lines[line_number]); line_number++;

		generator_parameters.invocation = stoi(lines[line_number]); line_number++;

		generator_parameters.resolution_step      = stof(lines[line_number]); line_number++;
		generator_parameters.generation_threshold = stof(lines[line_number]); line_number++;

		generator_parameters.min_surface_value    = stoi(lines[line_number]); line_number++;
		generator_parameters.max_surface_value    = stoi(lines[line_number]); line_number++;

////QMessageBox::information(NULL, "Import voxel generation model", "Here 001: " + std::string::number(line_number), //QMessageBox::Ok);

		if (!FW::stringtools::contains(lines[line_number], FLOAT_BLOCK_START)) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find start of float variable data to import float variables", //QMessageBox::Ok);
			return false;
		}

		line_number++;
		while (!FW::stringtools::contains(lines[line_number], FLOAT_BLOCK_END)) {
			if (line_number + 4 > lines.size() ) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Have function float variable data incorrectly defined.", //QMessageBox::Ok);
////QMessageBox::information(NULL, "Import voxel generation model", "Here 002 : " + std::string::number(line_number), //QMessageBox::Ok);
				return false;
			}

			voxel_generator_parameter_variable_struct_type variable;
			variable.active_variable      = stoi(lines[line_number]); line_number++;
			line =  lines[line_number];   line = FW::stringtools::truncate(line, line.size());
			variable.variable_name        = line; line_number++;
			variable.value                = stof(lines[line_number]); line_number++;
			variable.variable_step        = stof(lines[line_number]); line_number++;
			variable.active_variable_step = stoi(lines[line_number]); line_number++;
////QMessageBox::information(NULL, "Import voxel generation model", "Here 003 : " + std::string::number(line_number), //QMessageBox::Ok);
			generator_parameters.variables.push_back(variable);
////QMessageBox::information(NULL, "Import voxel generation model", "Here 004 : " + lines[line_number], //QMessageBox::Ok);
		}

		line_number++;

		if (!FW::stringtools::contains(lines[line_number], INT_BLOCK_START)) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find start of integer variable data to import integer variables", //QMessageBox::Ok);
			return false;
		}

		line_number++;
		while (!FW::stringtools::contains(lines[line_number], INT_BLOCK_END)) {
			if (line_number + 4 > lines.size()) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Have function integer variable data incorrectly defined.", //QMessageBox::Ok);
////QMessageBox::information(NULL, "Import voxel generation model", "Here 002 : " + std::string::number(line_number), //QMessageBox::Ok);
				return false;
			}

			voxel_generator_parameter_int_variable_struct_type int_variable;
			int_variable.active_variable      = stoi(lines[line_number]); line_number++;
			line =  lines[line_number];  line = FW::stringtools::truncate(line, line.size());
			int_variable.variable_name        = line; line_number++;
			int_variable.value                = stoi(lines[line_number]); line_number++;
			int_variable.variable_step        = stoi(lines[line_number]); line_number++;
			int_variable.active_variable_step = stoi(lines[line_number]); line_number++;
////QMessageBox::information(NULL, "Import voxel generation model", "Here 003 : " + std::string::number(line_number), //QMessageBox::Ok);
			generator_parameters.int_variables.push_back(int_variable);
////QMessageBox::information(NULL, "Import voxel generation model", "Here 004 : " + lines[line_number], //QMessageBox::Ok);
		}

		line_number++;
		if (!FW::stringtools::contains(lines[line_number], BOOL_BLOCK_START)) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find start of integer variable data to import integer variables", //QMessageBox::Ok);
			return false;
		}

		line_number++;
		while (!FW::stringtools::contains(lines[line_number], BOOL_BLOCK_END)) {
			if (line_number + 2 > lines.size()) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Have function integer variable data incorrectly defined.", //QMessageBox::Ok);
////QMessageBox::information(NULL, "Import voxel generation model", "Here 002 : " + std::string::number(line_number), //QMessageBox::Ok);
				return false;
			}

			voxel_generator_parameter_bool_variable_struct_type bool_variable;
			bool_variable.active_variable      = stoi(lines[line_number]); line_number++;
			line =  lines[line_number];   line = FW::stringtools::truncate(line, line.size());
			bool_variable.variable_name        = line; line_number++;
			bool_variable.value                = stoi(lines[line_number]); line_number++;
////QMessageBox::information(NULL, "Import voxel generation model", "Here 003 : " + std::string::number(line_number), //QMessageBox::Ok);
			generator_parameters.bool_variables.push_back(bool_variable);
////QMessageBox::information(NULL, "Import voxel generation model", "Here 004 : " + lines[line_number], //QMessageBox::Ok);
		}

		return true;
	}