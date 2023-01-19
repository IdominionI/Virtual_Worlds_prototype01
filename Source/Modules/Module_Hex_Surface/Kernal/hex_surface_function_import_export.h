#pragma once

#include <fstream>
#include <string>

#include "../Hex_surface_object/DataTypes/dt_hex_surface_generator.h"

#define OVER_WRITE_FUNCTION_FILE    100
#define READ_FUNCTION_FILE          101

#define STATIC        1000
#define DYNAMIC       1001
#define WORKING_MODEL 1002

#define BLOCK_START "["
#define BLOCK_END   "]"

#define FLOAT_BLOCK_START "<"
#define FLOAT_BLOCK_END   ">"

#define INT_BLOCK_START "{"
#define INT_BLOCK_END   "}"

#define BOOL_BLOCK_START "("
#define BOOL_BLOCK_END   ")"

#define ENDL '\n' // endl depreciated in QT6

/*
	voxel_function_import_export_class

	C++ Class that handles the import and export of hcp hex_surface point cloud parameter data 
	from and to a formatted text file so as for the user to save and reuse paramaters without
	having to reenter the data from scratch.

	Method of file format to read/write this data is to flag each parameter witha <>_BLOCK_START flag
	on a line to indicate the fllowing lines up to a <>_BLOCK_END flag is a parameter. The parameter data
	is also in a particular order that dictates which line betweeen these flags corresponds to a 
	particular prarmeter value.

	In a similar design, the BLOCK_START flag gives an indication that all lines up to a
	BLOCK_END flag are a set of paramater data block that have individual parameter data
	values bounded by <>_BLOCK_START and <>_BLOCK_END flags.
*/


class hex_surface_function_import_export_class {
public:
	std::fstream stream;
	std::string  input_line;

	hex_surface_generator_parameters_struct_type hex_surface_generator_parameters;

	int line_number = 0;

	int open_file_stream(std::string filename, int write_mode);
	void close_file_stream();
	bool save_hex_surface_generated_function(std::string file_pathname);
	bool import_hex_surface_genereated_function(hex_surface_generator_parameters_struct_type& generator_parameters, std::string file_pathname);
	bool read_expression_into_hex_surface_generator_parameters(std::string working_model_string, hex_surface_generator_parameters_struct_type& generator_parameters);

};
