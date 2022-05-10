#pragma once
#include <fstream>
#include <string>
#include <vector>

#include <Universal/Framework/FWstring_tools.h>

#include "../hex_surface_object/DataTypes/dt_hex_surface_automata.h"

#define OVER_WRITE_FUNCTION_FILE    100
#define READ_FUNCTION_FILE          101

#define RULE_DATA_START "["
#define RULE_DATA_END   "]"

#define RULE_BLOCK_START "{"
#define RULE_BLOCK_END   "}"

#define RULE_DELIMINATOR ":"

/*
	import_export_hex_surface_automata_rules_class

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

class import_export_hex_surface_automata_rules_class {
public:
	//QFile       file;
	//QTextStream stream;

	//std::string input_line;

	std::fstream stream;

	std::string input_line;

	int line_number = 0;

	std::vector <hex_surface_automata_rule_struct_type>  hex_surface_automata_rules;

	int open_file_stream(std::string filename, int write_mode) {

		if (write_mode == OVER_WRITE_FUNCTION_FILE) {
			stream.open(filename, std::ios::out | std::ios::out);

			if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!
		}
		else
			if (!stream.is_open()) return false;

		return true;
	}

	void close_file_stream() {
		stream.close();
	}

	bool export_hcp_automata_byte_rules(std::string file_pathname) {
/*		QFileDialog file_dialog;
		std::string file_pathname;

		file_pathname = file_dialog.getSaveFileName(NULL, "Save HEX hex surface Automata rules", "./", "automata hex surface rules (*AHSR.txt)");
//QMessageBox::information(NULL, "", file_pathname, QMessageBox::Ok);

		if (file_pathname.size() == 0) {
			QMessageBox::information(NULL, "", "No file name defined to save data to \n Save hex surface automata rules aborted", QMessageBox::Ok);
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) {
			QMessageBox::information(NULL, "", "Unable to open file\n" + file_pathname +"\n to save hex surface automata data to.", QMessageBox::Ok);
			return false;
		}

		stream << RULE_DATA_START << endl;

		write_hcp_automata_byte_rules();

		stream << RULE_DATA_END << endl;

		close_file_stream();

		return true;
*/

		if (file_pathname.size() == 0) {
			//QMessageBox::information(NULL, "", "No file name defined to save data to \n Save voxel automata byte rules aborted", //QMessageBox::Ok);
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) {
			//QMessageBox::information(NULL, "", "Unable to open file\n" + file_pathname +"\n to save automata data to.", //QMessageBox::Ok);
			return false;
		}

		stream << RULE_DATA_START << endl;

		write_hcp_automata_byte_rules();

		stream << RULE_DATA_END << endl;

		close_file_stream();

		return true;
	}

	void write_hcp_automata_byte_rules() {

		for(hex_surface_automata_rule_struct_type byte_automata_rule : hex_surface_automata_rules){
			stream << RULE_BLOCK_START << endl;

			stream << byte_automata_rule.name        << endl;
			stream << byte_automata_rule.active_rule << endl;
			stream << byte_automata_rule.hex_state   << endl;
			stream << byte_automata_rule.start_step  << endl;
			stream << byte_automata_rule.end_step    << endl;

			stream << byte_automata_rule.hex_self_rule_condition.lower_rule_value << RULE_DELIMINATOR;
			switch (byte_automata_rule.hex_self_rule_condition.lower_rule) {
				case hex_lower_rule_condition_enum_type::ignore : stream << HEX_NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
				case hex_lower_rule_condition_enum_type::LT     : stream << HEX_NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
				case hex_lower_rule_condition_enum_type::LTEQ   : stream << HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR;break;
			}

			switch (byte_automata_rule.hex_self_rule_condition.upper_rule) {
				case hex_upper_rule_condition_enum_type::ignore : stream << HEX_NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
				case hex_upper_rule_condition_enum_type::NE     : stream << HEX_NEIGHBOR_NOT_EQUAL_RULE_CODE << RULE_DELIMINATOR;         break;
				case hex_upper_rule_condition_enum_type::EQ     : stream << HEX_NEIGHBOR_EQUAL_RULE_CODE << RULE_DELIMINATOR;             break;
				case hex_upper_rule_condition_enum_type::LT     : stream << HEX_NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
				case hex_upper_rule_condition_enum_type::LTEQ   : stream << HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR;break;
			}
			stream << byte_automata_rule.hex_self_rule_condition.upper_rule_value << endl;

			for(int i=0;i<NUMBER_HEX_NEIGHBOURS;i++) {
				stream << byte_automata_rule.neighbours_rule_condition[i].lower_rule_value << RULE_DELIMINATOR;
				switch (byte_automata_rule.neighbours_rule_condition[i].lower_rule) {
					case hex_lower_rule_condition_enum_type::ignore : stream << HEX_NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
					case hex_lower_rule_condition_enum_type::LT     : stream << HEX_NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
					case hex_lower_rule_condition_enum_type::LTEQ   : stream << HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR;break;
				}

				switch (byte_automata_rule.neighbours_rule_condition[i].upper_rule) {
					case hex_upper_rule_condition_enum_type::ignore : stream << HEX_NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
					case hex_upper_rule_condition_enum_type::NE     : stream << HEX_NEIGHBOR_NOT_EQUAL_RULE_CODE << RULE_DELIMINATOR;         break;
					case hex_upper_rule_condition_enum_type::EQ     : stream << HEX_NEIGHBOR_EQUAL_RULE_CODE << RULE_DELIMINATOR;             break;
					case hex_upper_rule_condition_enum_type::LT     : stream << HEX_NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
					case hex_upper_rule_condition_enum_type::LTEQ   : stream << HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR;break;
				}
				stream << byte_automata_rule.neighbours_rule_condition[i].upper_rule_value << endl;
			}

			stream << RULE_BLOCK_END << endl;
		}
	}

	// -------------------------------------------------------------------

	bool open_file_to_read(std::string filename) {
		stream.open(filename, std::ios::in);

		if (!stream)
			return false;
		else
			//if (!file.isOpen()) return false;
			if (!stream.is_open()) return false;

		return true;
	}

	bool import_hcp_automata_byte_rules(std::vector <hex_surface_automata_rule_struct_type>  &automata_byte_rules_parameters,std::string file_pathname) {
		if (file_pathname.size() == 0) {
			//QMessageBox::information(NULL, "", "No file name defined to import data from \n Import automata byte rules aborted", //QMessageBox::Ok);
			return false;
		}

		if (!open_file_to_read(file_pathname)) {
			//QMessageBox::information(NULL, "", "Unable to open file\n" + file_pathname +"\n to import automata data to.", //QMessageBox::Ok);
			return false;
		}

		automata_byte_rules_parameters.clear();
		automata_byte_rules_parameters.shrink_to_fit();

		read_automata_byte_rules_into_byte_rules_parameters(file_pathname, automata_byte_rules_parameters);

		stream.close();
		return true;
	}

	bool read_automata_byte_rules_into_byte_rules_parameters(std::string file_pathname,std::vector <hex_surface_automata_rule_struct_type> &generator_parameters) {
		std::string line;
		int line_number = 0;

		// First line read must be a START_HEADER_FLAG
		line_number++;
		if (!read_file_line_data()) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Failed to read data from file at line " + std::string::number(line_number) + " of file \n" +file_pathname, //QMessageBox::Ok);
			return false;
		}
		////QMessageBox::information(NULL,"Import Automata byte rules","Import Automata byte rules 00 :"+input_line+":", //QMessageBox::Ok);
				//if (input_line.indexOf(RULE_DATA_START) == (std::string::npos)) {
		if (input_line.find(RULE_DATA_START) == (std::string::npos)) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			return false;
		}

		line_number++;
		if (!read_file_line_data()) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname, //QMessageBox::Ok);
			return false;
		}
		////QMessageBox::information(NULL,"Import Automata byte rules01 ",input_line, //QMessageBox::Ok);
				//while (input_line.indexOf(RULE_DATA_END) == (std::string::npos) && !file.atEnd()) {
		while (input_line.find(RULE_DATA_END) == (std::string::npos) && !stream.eof()) {

			hex_surface_automata_rule_struct_type hex_surface_automata_rule;
			if (!read_automata_byte_rule(file_pathname, hex_surface_automata_rule)) return false;

			generator_parameters.push_back(hex_surface_automata_rule);

			line_number++;
			if (!read_file_line_data()) {
				//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname, //QMessageBox::Ok);
				return false;
			}
		}

		return true;
	}

	bool read_automata_byte_rule(std::string file_pathname, hex_surface_automata_rule_struct_type &hex_surface_automata_rule) {
		// next line read must be a RULE_BLOCK_START flag
		if (input_line.find(RULE_BLOCK_START) == (std::string::npos)) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing RULE_BLOCK_START flag", //QMessageBox::Ok);
			return false;
		}

		//next 5 lines must have rule name,active_rule,voxel_state,start_step,end_step in that order
		line_number++;
		if (!read_file_line_data()) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
			return false;
		}
		hex_surface_automata_rule.name = input_line;

		line_number++;
		if (!read_file_line_data()) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
			return false;
		}
		if(stoi(input_line) == 0) hex_surface_automata_rule.active_rule = false; else hex_surface_automata_rule.active_rule = true;

		line_number++;
		if (!read_file_line_data()) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
			return false;
		}
		hex_surface_automata_rule.hex_state = stof(input_line);

		line_number++;
		if (!read_file_line_data()) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
			return false;
		}
		hex_surface_automata_rule.start_step = stoi(input_line);

		line_number++;
		if (!read_file_line_data()) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
			return false;
		}
		hex_surface_automata_rule.end_step = stoi(input_line);


		// next line contains the self rule definitions where each rule defenition is seperated by the deliminator RULE_DELIMINATOR
		line_number++;
		if (!read_file_line_data()) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
			return false;
		}
		//std::stringList rule_list = input_line.split(RULE_DELIMINATOR);

		std::vector<std::string> rule_list;
		rule_list = FW::stringtools::split(input_line, RULE_DELIMINATOR);

		if(rule_list.size()<4){
			//QMessageBox::information(NULL,"Import hex surface Automata rules", "Error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n"+"Incorrect parameters for self rule", QMessageBox::Ok);
			return false;
		}

		//hex_surface_automata_rule.hex_self_rule_condition.lower_rule_value = rule_list.value(0).toInt();
		hex_surface_automata_rule.hex_self_rule_condition.lower_rule_value = stoi(rule_list[0]);
		//switch (rule_list.value(1).toInt()) {
		switch (stoi(rule_list[1])) {
			case HEX_NEIGHBOR_IGNOR_RULE_CODE              : hex_surface_automata_rule.hex_self_rule_condition.lower_rule = hex_lower_rule_condition_enum_type::ignore; break;
			case HEX_NEIGHBOR_LESS_THAN_RULE_CODE          : hex_surface_automata_rule.hex_self_rule_condition.lower_rule = hex_lower_rule_condition_enum_type::LT;     break;
			case HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE : hex_surface_automata_rule.hex_self_rule_condition.lower_rule = hex_lower_rule_condition_enum_type::LTEQ;   break;
		}

		//switch (rule_list.value(2).toInt()) {
		switch (stoi(rule_list[2])) {
			case HEX_NEIGHBOR_IGNOR_RULE_CODE              : hex_surface_automata_rule.hex_self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::ignore; break;
			case HEX_NEIGHBOR_NOT_EQUAL_RULE_CODE          : hex_surface_automata_rule.hex_self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::NE;     break;
			case HEX_NEIGHBOR_EQUAL_RULE_CODE              : hex_surface_automata_rule.hex_self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::EQ;     break;
			case HEX_NEIGHBOR_LESS_THAN_RULE_CODE          : hex_surface_automata_rule.hex_self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::LT;    break;
			case HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE : hex_surface_automata_rule.hex_self_rule_condition.upper_rule = hex_upper_rule_condition_enum_type::LTEQ;   break;
		}
		//hex_surface_automata_rule.hex_self_rule_condition.upper_rule_value = rule_list.value(3).toInt();
		hex_surface_automata_rule.hex_self_rule_condition.upper_rule_value = stoi(rule_list[3]);


		// next NUMBER_HEX_NEIGHBOURS lines must have the neighbour activity states for the rule
		for (int i = 0; i < NUMBER_HEX_NEIGHBOURS; i++){
			line_number++;
			if(!read_file_line_data())  {
				//QMessageBox::information(NULL,"Import hex surface Automata rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", QMessageBox::Ok);
				return false;
			}

			//rule_list = input_line.split(RULE_DELIMINATOR);
			std::vector<std::string> rule_list;
			rule_list = FW::stringtools::split(input_line, RULE_DELIMINATOR);

			if(rule_list.size()<4){
				//QMessageBox::information(NULL,"Import hex surface Automata rules", "Error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n"+"Incorrect parameters for self rule", QMessageBox::Ok);
				return false;
			}

			hex_surface_automata_rule.neighbours_rule_condition[i].lower_rule_value = stoi(rule_list[0]);
			switch (stoi(rule_list[1])) {
				case HEX_NEIGHBOR_IGNOR_RULE_CODE              : hex_surface_automata_rule.neighbours_rule_condition[i].lower_rule = hex_lower_rule_condition_enum_type::ignore; break;
				case HEX_NEIGHBOR_LESS_THAN_RULE_CODE          : hex_surface_automata_rule.neighbours_rule_condition[i].lower_rule = hex_lower_rule_condition_enum_type::LT;     break;
				case HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE : hex_surface_automata_rule.neighbours_rule_condition[i].lower_rule = hex_lower_rule_condition_enum_type::LTEQ;   break;
			}

			switch (stoi(rule_list[2])) {
				case HEX_NEIGHBOR_IGNOR_RULE_CODE              : hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::ignore; break;
				case HEX_NEIGHBOR_NOT_EQUAL_RULE_CODE          : hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::NE;     break;
				case HEX_NEIGHBOR_EQUAL_RULE_CODE              : hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::EQ;     break;
				case HEX_NEIGHBOR_LESS_THAN_RULE_CODE          : hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::LT;     break;
				case HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE : hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule = hex_upper_rule_condition_enum_type::LTEQ;   break;
			}
			hex_surface_automata_rule.neighbours_rule_condition[i].upper_rule_value = stoi(rule_list[3]);
		}

		// next line read must be a RULE_BLOCK_END flag
		line_number++;
		if(!read_file_line_data())  {
			//QMessageBox::information(NULL,"Import hex surface Automata rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", QMessageBox::Ok);
			return false;
		}
		if (input_line.find(RULE_BLOCK_END) == (std::string::npos)) {
			//QMessageBox::information(NULL,"Import hex surface Automata rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing Rule block end flag", QMessageBox::Ok);
			return false;
		}

		return true;
	}



private:
	//std::string endl = '\n';

	bool read_file_line_data() {
		if (stream.eof()) return false;

		std::getline(stream, input_line);

		return true;
	}

};
