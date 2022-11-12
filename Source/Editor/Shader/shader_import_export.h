#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <Universal/Framework/FWstring_tools.h>
#include <Universal/Framework/FWfile_tools.h>

//#include <Source/Graphics_Engine/Shader/shader_parameters.h>
#include <Source/Graphics_Engine/Shader/shader_components.h>

#define OVER_WRITE_SHADER_FILE    100
#define READ_SHADER_FILE          101

#define SHADER_BLOCK_START "["
#define SHADER_BLOCK_END   "]"

#define FLOAT_SHADER_BLOCK_START "<"
#define FLOAT_SHADER_BLOCK_END   ">"

#define INT_SHADER_BLOCK_START "{"
#define INT_SHADER_BLOCK_END   "}"

#define BOOL_SHADER_BLOCK_START "("
#define BOOL_SHADER_BLOCK_END   ")"

//#define ENDL Qt::endl // endl depreciated in QT6
#define ENDL '\n' // endl depreciated in QT6

/*
				SHADER PARAMETER IMPORT EXPORT

	Class that handles the import and export of shader uniform parameter data
	from and to an ASCII text file for all application data types.

	Shader data is defined by various data blocks for each data type, and
	for the entire shader parameter data itself. The shader variables data is
	of a given order and constant in the number of parameters per shader
	variable. Any change of this order and number of lines per variable type
	will result in a error.
*/

class voxel_texture_import_export_class {
public:
	std::fstream stream;

	std::string input_line;

	//shader_parameters_struct_type shader_parameters;
	material_struct_type shader_parameters;

	int line_number = 0;

	int open_file_stream(std::string filename, int write_mode) {

		if (write_mode == OVER_WRITE_SHADER_FILE  ) {
			stream.open(filename, std::ios::out);

			if (!stream) return false;
		}
		else
			if (!stream.is_open()) return false;

		return true;
	}

	void close_file_stream() {
		stream.close();
	}

	bool save_working_generated_function(std::string file_pathname) {
////QMessageBox::information(NULL, "", "in save_working_generated_function", //QMessageBox::Ok);
		//QFileDialog file_dialog;
		//std::string file_pathname;

		//file_pathname = file_dialog.getSaveFileName(NULL, "Save Static Generated Function", "./", "Texture working model (*.twm)");
////QMessageBox::information(NULL, "", file_pathname, //QMessageBox::Ok);

		if (file_pathname.size() == 0) {
//QMessageBox::information(NULL, "", "No file name defined to save data to \n Save Static Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_SHADER_FILE  )) return false;

		// ****** Shader files *******
		stream << SHADER_BLOCK_START << ENDL;

		stream << shader_parameters.vertex_shader_file_pathname.string()           << ENDL;
		stream << shader_parameters.point_shader_file_pathname.string()            << ENDL;
		stream << shader_parameters.geometry_shader_file_pathname.string()         << ENDL;
		stream << shader_parameters.fragment_shader_file_pathname.string()         << ENDL;
		stream << shader_parameters.default_vertex_shader_file_pathname.string()   << ENDL;
		stream << shader_parameters.default_point_shader_file_pathname.string()    << ENDL;
		stream << shader_parameters.default_geometry_shader_file_pathname.string() << ENDL;
		stream << shader_parameters.default_fragment_shader_file_pathname.string() << ENDL;

		stream << SHADER_BLOCK_END << ENDL;
		// ***************************

		stream << FLOAT_SHADER_BLOCK_START << ENDL;

		for (shader_parameter_variable_struct_type variable : shader_parameters.variables) {
			stream << variable.active_variable        << ENDL;
			stream << variable.variable_name.c_str()  << ENDL;
			stream << variable.value				  << ENDL;
			stream << variable.variable_step          << ENDL;
			stream << variable.active_variable_step   << ENDL;
			stream << variable.slider_min             << ENDL;
			stream << variable.slider_max             << ENDL;
		}

		stream << FLOAT_SHADER_BLOCK_END << ENDL;

		stream << INT_SHADER_BLOCK_START << ENDL;

		for (shader_parameter_int_variable_struct_type variable : shader_parameters.int_variables) {
			stream << variable.active_variable       << ENDL;
			stream << variable.variable_name.c_str() << ENDL;
			stream << variable.value                 << ENDL;
			stream << variable.variable_step         << ENDL;
			stream << variable.active_variable_step  << ENDL;
			stream << variable.slider_min            << ENDL;
			stream << variable.slider_max            << ENDL;
		}

		stream << INT_SHADER_BLOCK_END    << ENDL;

		stream << BOOL_SHADER_BLOCK_START << ENDL;

		for (shader_parameter_bool_variable_struct_type variable : shader_parameters.bool_variables) {
			stream << variable.active_variable       << ENDL;
			stream << variable.variable_name.c_str() << ENDL;
			stream << variable.value                 << ENDL;
		}

		stream << BOOL_SHADER_BLOCK_END << ENDL;

		close_file_stream();

		return true;
	}

	//bool import_voxel_genereated_function(shader_parameters_struct_type& texture_parameters, std::string file_pathname) {
	bool import_voxel_genereated_function(material_struct_type *texture_parameters, std::string file_pathname) {
//QMessageBox::information(NULL, "", file_pathname, //QMessageBox::Ok);

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

		// Read entire file working_model_file text contents into stringworking_model_string
		std::string working_model_string = FW::filetools::read_all(working_model_file);

		return read_expression_into_shader_parameters(working_model_string, texture_parameters);
	}

	//bool read_expression_into_shader_parameters(std::string working_model_string, shader_parameters_struct_type& texture_parameters) {
	bool read_expression_into_shader_parameters(std::string working_model_string, material_struct_type *texture_parameters) {
//QStringList lines = working_model_string.split('\n');// Create a list of strings for each line in the expression code
		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');
		std::string line;
		int line_number = 0;

		// Find the line or index number in the list of lines of the expression that has the expression variable list defined 
		while (!FW::stringtools::contains(lines[line_number], SHADER_BLOCK_START))
			line_number++;

		// If have not found a defined expression return an error
		if (line_number >= lines.size()) {
			//QMessageBox::information(NULL, "Import voxel texture model", "Import voxel  texture model ERROR : \n Could not find start block to import unser shader files.", //QMessageBox::Ok);
			return false;
		}

		if (lines.size() < line_number + 9) {
			//QMessageBox::information(NULL, "Import voxel texture model", "Import voxel  texture model ERROR : \n Corrupted file.", //QMessageBox::Ok);
			return false;
		}

		line_number++;

		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->vertex_shader_file_pathname = line; line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->point_shader_file_pathname = line; line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->geometry_shader_file_pathname = line; line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->fragment_shader_file_pathname = line; line_number++;

		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->default_vertex_shader_file_pathname = line; line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->default_point_shader_file_pathname = line; line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->default_geometry_shader_file_pathname = line; line_number++;
		line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
		texture_parameters->default_fragment_shader_file_pathname = line; line_number++;

		line_number++;

		// Find the line or index number in the list of lines of the expression that has the expression variable list defined 
		while (!FW::stringtools::contains(lines[line_number], FLOAT_SHADER_BLOCK_START))
			line_number++;

		// If have not found a defined expression return an error
		if (line_number >= lines.size()) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find start block to import parameter data.", //QMessageBox::Ok);
			return false;
		}

		line_number++;
//QMessageBox::information(NULL, "Import voxel generation model", "Here 001: " + std::string::number(line_number), //QMessageBox::Ok);

		while (!FW::stringtools::contains(lines[line_number], FLOAT_SHADER_BLOCK_END)) {
			if (line_number + 8 > lines.size()) {
				//QMessageBox::information(NULL, "Import voxel texture model", "Import voxel texture model ERROR : \n Have function variable data incorrectly defined.", //QMessageBox::Ok);
////QMessageBox::information(NULL, "Import voxel generation model", "Here 002 : " + std::string::number(line_number), //QMessageBox::Ok);
				return false;
			}

			shader_parameter_variable_struct_type variable;
			variable.active_variable = stof(lines[line_number]); line_number++;
			line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
			variable.variable_name = line; line_number++;
			variable.value = stof(lines[line_number]); line_number++;
			variable.variable_step = stof(lines[line_number]); line_number++;
			variable.active_variable_step = stof(lines[line_number]); line_number++;
			variable.slider_min = stof(lines[line_number]);   line_number++;
			variable.slider_max = stof(lines[line_number]);   line_number++;

////QMessageBox::information(NULL, "Import voxel generation model", "Here 003 : " + std::string::number(line_number), //QMessageBox::Ok);
			texture_parameters->variables.push_back(variable);
////QMessageBox::information(NULL, "Import voxel generation model", "Here 004 : " + lines[line_number], //QMessageBox::Ok);
		}

		// Find the line or index number in the list of lines of the expression that has the expression integer variable list defined 
		while (!FW::stringtools::contains(lines[line_number], INT_SHADER_BLOCK_START))
			line_number++;

		// If have not found a defined expression return an error
		if (line_number >= lines.size()) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import virtual painter generation model ERROR : \n Could not find start block to import integer parameter data.", //QMessageBox::Ok);
			return false;
		}

		line_number++;

		////QMessageBox::information(NULL, "Import voxel generation model", "Here 001: " + std::string::number(line_number), //QMessageBox::Ok);

		while (!FW::stringtools::contains(lines[line_number], INT_SHADER_BLOCK_END)) {
			if (line_number + 8 > lines.size()) {
				//QMessageBox::information(NULL, "Import voxel texture model", "Import virtual painter parameter ERROR : \n Have integer variable data incorrectly defined.", //QMessageBox::Ok);
				////QMessageBox::information(NULL, "Import voxel generation model", "Here 002 : " + std::string::number(line_number), //QMessageBox::Ok);
				return false;
			}

			shader_parameter_int_variable_struct_type variable;
			variable.active_variable = stoi(lines[line_number]); line_number++;
			line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
			variable.variable_name = line; line_number++;
			variable.value = stoi(lines[line_number]); line_number++;
			variable.variable_step = stoi(lines[line_number]); line_number++;
			variable.active_variable_step = stoi(lines[line_number]); line_number++;
			variable.slider_min = stoi(lines[line_number]); line_number++;
			variable.slider_max = stoi(lines[line_number]); line_number++;

//QMessageBox::information(NULL, "Import voxel generation model", "Here 003 : " + std::string::number(line_number), //QMessageBox::Ok);
			texture_parameters->int_variables.push_back(variable);
//QMessageBox::information(NULL, "Import voxel generation model", "Here 004 : " + lines[line_number], //QMessageBox::Ok);
		}


		// Find the line or index number in the list of lines of the expression that has the expression boolean variable list defined 
		while (!FW::stringtools::contains(lines[line_number], BOOL_SHADER_BLOCK_START))
			line_number++;

		// If have not found a defined expression return an error
		if (line_number >= lines.size()) {
//QMessageBox::information(NULL, "Import voxel generation model", "Import virtual painter generation model ERROR : \n Could not find start block to import boolean parameter data.", //QMessageBox::Ok);
			return false;
		}

		line_number++;

//QMessageBox::information(NULL, "Import voxel generation model", "Here 001: " + std::string::number(line_number), //QMessageBox::Ok);

		while (!FW::stringtools::contains(lines[line_number], BOOL_SHADER_BLOCK_END)) {
			if (line_number + 4 > lines.size()) {
//QMessageBox::information(NULL, "Import voxel texture model", "Import virtual painter parameter ERROR : \n Have boolean variable data incorrectly defined.", //QMessageBox::Ok);
//QMessageBox::information(NULL, "Import voxel generation model", "Here 002 : " + std::string::number(line_number), //QMessageBox::Ok);
				return false;
			}

			shader_parameter_bool_variable_struct_type variable;
			variable.active_variable = stoi(lines[line_number]); line_number++;
			line = lines[line_number];  line = FW::stringtools::truncate(line, line.size());
			variable.variable_name = line; line_number++;
			variable.value = stoi(lines[line_number]); line_number++;

//QMessageBox::information(NULL, "Import voxel generation model", "Here 003 : " + std::string::number(line_number), //QMessageBox::Ok);
			texture_parameters->bool_variables.push_back(variable);
//QMessageBox::information(NULL, "Import voxel generation model", "Here 004 : " + lines[line_number], //QMessageBox::Ok);
		}

		return true;
	}



};
