#include "automata_byte_import_export.h"

#include <Universal/Framework/FWstring_tools.h>

#define ENDL '\n'

int import_export_byte_automata_rules_class::open_file_stream(std::string filename, int write_mode) {

	if (write_mode == OVER_WRITE_FUNCTION_FILE) {
		stream.open(filename, std::ios::out | std::ios::out);

		if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!
	}
	else
		if (!stream.is_open()) return false;

	return true;
}

void import_export_byte_automata_rules_class::close_file_stream() {
	stream.close();
}

bool import_export_byte_automata_rules_class::export_hcp_automata_byte_rules(std::string file_pathname) {
	////QMessageBox::information(NULL, "", file_pathname, //QMessageBox::Ok);

	if (file_pathname.size() == 0) {
		//QMessageBox::information(NULL, "", "No file name defined to save data to \n Save voxel automata byte rules aborted", //QMessageBox::Ok);
		return false;
	}

	if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) {
		//QMessageBox::information(NULL, "", "Unable to open file\n" + file_pathname +"\n to save automata data to.", //QMessageBox::Ok);
		return false;
	}

	stream << RULE_DATA_START << ENDL;

	write_hcp_automata_byte_rules();

	stream << RULE_DATA_END << ENDL;

	close_file_stream();

	return true;
}

void import_export_byte_automata_rules_class::write_hcp_automata_byte_rules() {

	for (voxel_hcp_automata_byte_rule_struct_type byte_automata_rule : byte_automata_rules) {
		stream << RULE_BLOCK_START << ENDL;

		stream << byte_automata_rule.name << ENDL;
		stream << byte_automata_rule.active_rule << ENDL;
		stream << byte_automata_rule.voxel_state << ENDL;
		stream << byte_automata_rule.start_step << ENDL;
		stream << byte_automata_rule.end_step << ENDL;

		stream << byte_automata_rule.voxel_self_rule_condition.lower_rule_value << RULE_DELIMINATOR;
		switch (byte_automata_rule.voxel_self_rule_condition.lower_rule) {
		case lower_rule_condition_enum_type::ignore: stream << NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
		case lower_rule_condition_enum_type::LT: stream << NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
		case lower_rule_condition_enum_type::LTEQ: stream << NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR; break;
		}

		switch (byte_automata_rule.voxel_self_rule_condition.upper_rule) {
		case upper_rule_condition_enum_type::ignore: stream << NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
		case upper_rule_condition_enum_type::NE: stream << NEIGHBOR_NOT_EQUAL_RULE_CODE << RULE_DELIMINATOR;         break;
		case upper_rule_condition_enum_type::EQ: stream << NEIGHBOR_EQUAL_RULE_CODE << RULE_DELIMINATOR;             break;
		case upper_rule_condition_enum_type::LT: stream << NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
		case upper_rule_condition_enum_type::LTEQ: stream << NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR; break;
		}
		stream << byte_automata_rule.voxel_self_rule_condition.upper_rule_value << ENDL;

		for (int i = 0; i < NUMBER_HCP_NEIGHBOURS; i++) {
			stream << byte_automata_rule.neighbours_rule_condition[i].lower_rule_value << RULE_DELIMINATOR;
			switch (byte_automata_rule.neighbours_rule_condition[i].lower_rule) {
			case lower_rule_condition_enum_type::ignore: stream << NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
			case lower_rule_condition_enum_type::LT: stream << NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
			case lower_rule_condition_enum_type::LTEQ: stream << NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR; break;
			}

			switch (byte_automata_rule.neighbours_rule_condition[i].upper_rule) {
			case upper_rule_condition_enum_type::ignore: stream << NEIGHBOR_IGNOR_RULE_CODE << RULE_DELIMINATOR;             break;
			case upper_rule_condition_enum_type::NE: stream << NEIGHBOR_NOT_EQUAL_RULE_CODE << RULE_DELIMINATOR;         break;
			case upper_rule_condition_enum_type::EQ: stream << NEIGHBOR_EQUAL_RULE_CODE << RULE_DELIMINATOR;             break;
			case upper_rule_condition_enum_type::LT: stream << NEIGHBOR_LESS_THAN_RULE_CODE << RULE_DELIMINATOR;         break;
			case upper_rule_condition_enum_type::LTEQ: stream << NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE << RULE_DELIMINATOR; break;
			}
			stream << byte_automata_rule.neighbours_rule_condition[i].upper_rule_value << ENDL;
		}

		stream << RULE_BLOCK_END << ENDL;
	}
}

// -------------------------------------------------------------------

bool import_export_byte_automata_rules_class::open_file_to_read(std::string filename) {
	stream.open(filename, std::ios::in);

	if (!stream)
		return false;
	else
		if (!stream.is_open()) return false;

	return true;
}

bool import_export_byte_automata_rules_class::import_hcp_automata_byte_rules(std::vector <voxel_hcp_automata_byte_rule_struct_type>& automata_byte_rules_parameters, std::string file_pathname) {
	////QMessageBox::information(NULL, "", file_pathname, //QMessageBox::Ok);

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

bool import_export_byte_automata_rules_class::read_automata_byte_rules_into_byte_rules_parameters(std::string file_pathname, std::vector <voxel_hcp_automata_byte_rule_struct_type>& generator_parameters) {
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

		voxel_hcp_automata_byte_rule_struct_type voxel_hcp_automata_rule;
		if (!read_automata_byte_rule(file_pathname, voxel_hcp_automata_rule)) return false;

		generator_parameters.push_back(voxel_hcp_automata_rule);

		line_number++;
		if (!read_file_line_data()) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname, //QMessageBox::Ok);
			return false;
		}
	}

	return true;
}

bool import_export_byte_automata_rules_class::read_automata_byte_rule(std::string file_pathname, voxel_hcp_automata_byte_rule_struct_type& voxel_hcp_automata_rule) {

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
	voxel_hcp_automata_rule.name = input_line;

	line_number++;
	if (!read_file_line_data()) {
		//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
		return false;
	}
	if (stoi(input_line) == 0) voxel_hcp_automata_rule.active_rule = false; else voxel_hcp_automata_rule.active_rule = true;

	line_number++;
	if (!read_file_line_data()) {
		//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
		return false;
	}
	voxel_hcp_automata_rule.voxel_state = stoi(input_line);// .toUInt();

	line_number++;
	if (!read_file_line_data()) {
		//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
		return false;
	}
	voxel_hcp_automata_rule.start_step = stoi(input_line);// .toUInt();

	line_number++;
	if (!read_file_line_data()) {
		//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
		return false;
	}
	voxel_hcp_automata_rule.end_step = stoi(input_line);// .toUInt();


	// next line contains the self rule definitions where each rule defenintion is seperated by the deliminator RULE_DELIMINATOR
	line_number++;
	if (!read_file_line_data()) {
		//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
		return false;
	}

	std::vector<std::string> rule_list;
	rule_list = FW::stringtools::split(input_line, RULE_DELIMINATOR);

	if (rule_list.size() < 4) {
		//QMessageBox::information(NULL,"Import Automata byte rules", "Error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n"+"Incorrect parameters for self rule", //QMessageBox::Ok);
		return false;
	}


	voxel_hcp_automata_rule.voxel_self_rule_condition.lower_rule_value = stoi(rule_list[0]);

	switch (stoi(rule_list[1])) {
	case NEIGHBOR_IGNOR_RULE_CODE: voxel_hcp_automata_rule.voxel_self_rule_condition.lower_rule = lower_rule_condition_enum_type::ignore; break;
	case NEIGHBOR_LESS_THAN_RULE_CODE: voxel_hcp_automata_rule.voxel_self_rule_condition.lower_rule = lower_rule_condition_enum_type::LT;     break;
	case NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE: voxel_hcp_automata_rule.voxel_self_rule_condition.lower_rule = lower_rule_condition_enum_type::LTEQ;   break;
	}

	switch (stoi(rule_list[2])) {
	case NEIGHBOR_IGNOR_RULE_CODE: voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule = upper_rule_condition_enum_type::ignore; break;
	case NEIGHBOR_NOT_EQUAL_RULE_CODE: voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule = upper_rule_condition_enum_type::NE;     break;
	case NEIGHBOR_EQUAL_RULE_CODE: voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule = upper_rule_condition_enum_type::EQ;     break;
	case NEIGHBOR_LESS_THAN_RULE_CODE: voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule = upper_rule_condition_enum_type::LT;    break;
	case NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE: voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule = upper_rule_condition_enum_type::LTEQ;   break;
	}
	voxel_hcp_automata_rule.voxel_self_rule_condition.upper_rule_value = stoi(rule_list[3]);// .toInt();


	// next NUMBER_HCP_NEIGHBOURS lines must have the neighbour activity states for the rule
	for (int i = 0; i < NUMBER_HCP_NEIGHBOURS; i++) {
		line_number++;
		if (!read_file_line_data()) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
			return false;
		}


		std::vector<std::string> rule_list;
		rule_list = FW::stringtools::split(input_line, RULE_DELIMINATOR);

		if (rule_list.size() < 4) {
			//QMessageBox::information(NULL,"Import Automata byte rules", "Error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n"+"Incorrect parameters for self rule", //QMessageBox::Ok);
			return false;
		}

		voxel_hcp_automata_rule.neighbours_rule_condition[i].lower_rule_value = stoi(rule_list[0]);// .toInt();
		switch (stoi(rule_list[1])) {
		case NEIGHBOR_IGNOR_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].lower_rule = lower_rule_condition_enum_type::ignore; break;
		case NEIGHBOR_LESS_THAN_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].lower_rule = lower_rule_condition_enum_type::LT;     break;
		case NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].lower_rule = lower_rule_condition_enum_type::LTEQ;   break;
		}

		switch (stoi(rule_list[2])) {
		case NEIGHBOR_IGNOR_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule = upper_rule_condition_enum_type::ignore; break;
		case NEIGHBOR_NOT_EQUAL_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule = upper_rule_condition_enum_type::NE;     break;
		case NEIGHBOR_EQUAL_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule = upper_rule_condition_enum_type::EQ;     break;
		case NEIGHBOR_LESS_THAN_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule = upper_rule_condition_enum_type::LT;    break;
		case NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE: voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule = upper_rule_condition_enum_type::LTEQ;   break;
		}
		voxel_hcp_automata_rule.neighbours_rule_condition[i].upper_rule_value = stoi(rule_list[3]);// .toInt();
	}

	// next line read must be a RULE_BLOCK_END flag
	line_number++;
	if (!read_file_line_data()) {
		//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\n", //QMessageBox::Ok);
		return false;
	}
	if (input_line.find(RULE_BLOCK_END) == (std::string::npos)) {
		//QMessageBox::information(NULL,"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing Rule block end flag", //QMessageBox::Ok);
		return false;
	}

	return true;
}

//----------------------------------------------

bool import_export_byte_automata_rules_class::read_file_line_data() {
			if(stream.eof()) return false;

			std::getline(stream, input_line);

			return true;
		}
