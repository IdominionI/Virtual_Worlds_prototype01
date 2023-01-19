#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <Universal/Framework/FWfile_tools.h>
#include <Universal/Framework/FWstring_tools.h>


#define START_COMPUTE_FUNCTION_BLOCK_FLAG   "begin_function"
#define END_COMPUTE_FUNCTION_BLOCK_FLAG     "end_function"

#define START_COMPUTE_EXPRESSION_BLOCK_FLAG	"begin_expression"
#define END_COMPUTE_EXPRESSION_BLOCK_FLAG	"end_expression"

/*
	import_compute_expression_class

	C++ class that reads a user defined shader code from a text file where in that 
	text file the shader code is in the format 

	begin_function
		user created GLSL functions
	end_function

	begin_expression
	
		user created GLSL main function code to give an output value 

		output_result = output value ;
	
	end_expression

	Between begin_function and end_function are the user defined functions that are to be
	used in the main glsl source code that the user enters between begin_expression and 
	end_expression to create a value to output from the shader. The output_result must 
	be present to do this and is a reserved word.

	What this class does is extract the code between these two flag blocks as a separate 
	text strings to be later merged together as described in the Module Compute Generator 
	Class section.

	In the Visual Worlds application, all shaders, including the compute shader uses 
	this method to create the shader programs.
*/

class import_compute_expression_class {
public:
	std::string               filename_to_read;
	std::string               expression_name;
	std::vector<std::string>  expression_variables;
	std::string               expression_code = "";
	std::string               function_code = "\n"; // glsl code to be in the shader main function

	std::string  expression_string = "", input_line; // std::string class variables to store and process the text of the glsl source file
	int line_number = 0;

	bool import_compute_expression() {
//QMessageBox::information(NULL, "", "import_nodes HERE01\n : " + filename_to_read, QMessageBox::Ok);

		if (filename_to_read.size() == 0) {
			//QMessageBox::information(NULL, "", "No file name defined to import data from \n import_compute_expression aborted", //QMessageBox::Ok);
			return false;
		}

		std::fstream file(filename_to_read, std::ios::in);

		expression_string = FW::filetools::read_all(file);

		if (expression_string == "") {
			//QMessageBox::information(0, "Function Expression Error", "Function Expression Error :\n copute function text file \n" + filename_to_read + '\n' + "is empty.", QMessageBox::Ok);
			printf("Function Expression Error :\n compute function text file \n %s '\n' is empty.\n", filename_to_read.c_str());
			return false;
		}

		define_compute_expression();

		file.close();
		return true;
	}

	bool define_compute_expression() {
//QMessageBox::information(0, "Function Expression test", "Test 01\n"+ filename_to_read +'\n'+ expression_string, QMessageBox::Ok);
		//int line;

		if (!define_function_block_code()) return false; // Get user defined glsl function code
//printf(" Import of the compute file defined_function_block_code %s \n" , function_code.c_str());


		if (!define_expression_code()) return false;// Get user defined glsl main function expression code
//printf("  Import of the compute file defined_expression_block_code %s\n" , expression_code.c_str());

		return true;
	}

	bool define_function_block_code() {
		if (FW::stringtools::contains(expression_string,START_COMPUTE_FUNCTION_BLOCK_FLAG)) {    // look for start of function section flag in imported file
			if (!FW::stringtools::contains(expression_string,END_COMPUTE_FUNCTION_BLOCK_FLAG)) { // look for end of function section flag in imported file
				printf("ERROR : Import of the compute file \n %s \n has a function block with no end of function block flag.\n", filename_to_read.c_str());
				return false;
			}

			// Make sure end of function block does not preceed start of function block
			//if (expression_string.indexOf(END_COMPUTE_FUNCTION_BLOCK_FLAG, 0) < expression_string.indexOf(START_COMPUTE_FUNCTION_BLOCK_FLAG, 0)) {
			if (FW::stringtools::indexOf(expression_string,END_COMPUTE_FUNCTION_BLOCK_FLAG, 0) < FW::stringtools::indexOf(expression_string,START_COMPUTE_FUNCTION_BLOCK_FLAG, 0)) {
				printf("ERROR : Import of the compute file \n %s \n has a function block with end of function block flag occururing before the start function block flag.\n", filename_to_read.c_str());
				return false;
			}

			import_function_block();

			return true;
		}
		else {
			if (FW::stringtools::contains(expression_string,END_COMPUTE_FUNCTION_BLOCK_FLAG)) { // Have no begin function section flag, but do have end of function section flag
				printf("ERROR : Import of the compute file \n %s \n  has a function block with an end of function block flag, but no start of function block flag.\n", filename_to_read.c_str());
				return false;
			}
		}

		// have no function code to import but this is OK
		return true;
	}

	// Store all text between start and end of function flags in std::string class variable function_code
	void import_function_block() {
		int numchar = 0;
		std::string start_block_string = START_COMPUTE_FUNCTION_BLOCK_FLAG;
		numchar = FW::stringtools::indexOf(expression_string, END_COMPUTE_FUNCTION_BLOCK_FLAG, 0);
//printf("import_compute_expression_class :: import_function_block 000\n %s: %i \n", expression_string.c_str(), numchar);
		function_code = expression_string.substr(0,numchar);
		numchar = (FW::stringtools::indexOf(function_code,START_COMPUTE_FUNCTION_BLOCK_FLAG, 0) + int(start_block_string.length()));
//printf("import_compute_expression_class :: import_function_block 111 %s: %i %i %i\n",function_code.c_str(), function_code.length(), FW::stringtools::indexOf(function_code, START_COMPUTE_FUNCTION_BLOCK_FLAG, 0), start_block_string.length());
		function_code = function_code.substr(numchar, function_code.length());
//printf("import_compute_expression_class :: import_function_block 222: %s\n",function_code.c_str());
	}

	bool define_expression_code() {
		if (FW::stringtools::contains(expression_string, START_COMPUTE_EXPRESSION_BLOCK_FLAG)) {    // look for start of main function section flag in imported file
			if (!FW::stringtools::contains(expression_string,END_COMPUTE_EXPRESSION_BLOCK_FLAG)) { // look for end of main function section flag in imported file
				printf("ERROR : Import of the compute file \n %s \n  has a expression block with no end of expression block flag.\n", filename_to_read.c_str());
				return false;
			}

			// Make sure end of main function block does not preceed start of function block
			if (FW::stringtools::indexOf(expression_string, END_COMPUTE_EXPRESSION_BLOCK_FLAG, 0) < FW::stringtools::indexOf(expression_string,START_COMPUTE_EXPRESSION_BLOCK_FLAG, 0)) {
				printf("ERROR : Import of the compute file \n %s \n  has a expression block with end of expression block flag occururing before the start expression block flag.\n", filename_to_read.c_str());
				return false;
			}

			import_expression_block();

			return true;
		}
		else {
			if (FW::stringtools::contains(expression_string,END_COMPUTE_EXPRESSION_BLOCK_FLAG)) {// Have no begin main function section flag, but do have end of function section flag
				printf("ERROR : Import of the compute file \n %s \n  has a expression block with end of expression block flag occururing before the start expression block flag.\n", filename_to_read.c_str());
				return false;
			}
		}

		// have no main function expression code to import but this is OK
		return true;
	}

	// Store all text between start and end of main expression flags in std::string class variable expression_code
	void import_expression_block() {
		int numchar = 0;
		std::string start_block_string = START_COMPUTE_EXPRESSION_BLOCK_FLAG;

		numchar = FW::stringtools::indexOf(expression_string, END_COMPUTE_EXPRESSION_BLOCK_FLAG, 0);
//printf("import_compute_expression_class :: import_expression_block 000\n %s: %i \n", expression_string.c_str(), numchar);
		expression_code = expression_string.substr(0, numchar);
//printf("import_compute_expression_class :: import_function_block 111 %s: %i %i %i\n", expression_code.c_str(), expression_code.length(), FW::stringtools::indexOf(expression_code, START_COMPUTE_FUNCTION_BLOCK_FLAG, 0), start_block_string.length());

		numchar = (FW::stringtools::indexOf(expression_string, START_COMPUTE_EXPRESSION_BLOCK_FLAG, 0) + int(start_block_string.length()));
		expression_code = expression_code.substr(numchar, expression_code.length());
//printf("import_compute_expression_class :: import_function_block 222: %s\n", expression_code.c_str());
	}

};