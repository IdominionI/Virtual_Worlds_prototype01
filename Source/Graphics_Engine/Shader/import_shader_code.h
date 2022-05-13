#pragma once

#include <string>
#include <fstream>

#include <Universal/Framework/FWfile_tools.h>

/*
				import_shader_code_class

	C++ class to import OpenGL glsl shader code files into a 
	string class variable shader_string
*/


class import_shader_code_class {
public:
	std::string  shader_string = "", input_line;
	int line_number = 0;

	import_shader_code_class() {}

	// Import all the text contained in the file of pathname file_pathname_to_read into the
	// string class variable shader_string
	std::string import_code(std::string file_pathname_to_read) {
//QMessageBox::information(NULL, "", "import_nodes HERE01\n : " + filename_to_read, QMessageBox::Ok);

		std::fstream file(file_pathname_to_read, std::ios::in);

		if (!file.is_open()) return "";

		shader_string = FW::filetools::read_all(file);

		if (shader_string == "") {
			//QMessageBox::information(0, "import_shader_code_class", "import_shader_code_class Error :\n import_shader_code_class text file \n" + file_pathname_to_read + '\n' + "is empty.", QMessageBox::Ok);
			printf("import_shader_code_class Error :\n import_shader_code_class text file \n %s \n + is empty.", file_pathname_to_read);
		}

		file.close();
		return shader_string;
	}

protected:

private:
};