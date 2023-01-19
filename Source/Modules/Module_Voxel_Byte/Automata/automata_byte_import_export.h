#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "../Voxel_hcp_object/DataTypes/dt_voxel_automata.h"

#define OVER_WRITE_FUNCTION_FILE    100
#define READ_FUNCTION_FILE          101

#define RULE_DATA_START "["
#define RULE_DATA_END   "]"

#define RULE_BLOCK_START "{"
#define RULE_BLOCK_END   "}"

#define RULE_DELIMINATOR ":"

/*
	import_export_byte_automata_rules_class

	C++ Class that handles the import and export of cellula automata rule data from and to
	a formatted text file 

	Method of file format to read/write this data is to flag each cellula automata rule with
	a RULE_BLOCK_START on a line to indicate the fllowing lines up to a RULE_BLOCK_END flag is
	a cellula automata rule. The cellula automata rule is also in a particular order that
	dictates which line betweeen these flags corresponds to a particular cellula automata rule
	prarmeter.

	In a similar design, the RULE_DATA_START flag gives an indication that all lines up to a
	RULE_DATA_END flag are a set of cellula automata rules that have individual cellula automata
	rules bounded by RULE_BLOCK_START and RULE_BLOCK_END flags.

*/

class import_export_byte_automata_rules_class {
public:
	std::fstream stream;

	std::string input_line;

	int line_number = 0;

	std::vector <voxel_hcp_automata_byte_rule_struct_type>  byte_automata_rules;

	int open_file_stream(std::string filename, int write_mode);
	void close_file_stream();
	bool export_hcp_automata_byte_rules(std::string file_pathname);
	void write_hcp_automata_byte_rules();
	bool open_file_to_read(std::string filename);
	bool import_hcp_automata_byte_rules(std::vector <voxel_hcp_automata_byte_rule_struct_type>& automata_byte_rules_parameters, std::string file_pathname);
	bool read_automata_byte_rules_into_byte_rules_parameters(std::string file_pathname, std::vector <voxel_hcp_automata_byte_rule_struct_type>& generator_parameters);
	bool read_automata_byte_rule(std::string file_pathname, voxel_hcp_automata_byte_rule_struct_type& voxel_hcp_automata_rule);


private:
	bool read_file_line_data();

};
