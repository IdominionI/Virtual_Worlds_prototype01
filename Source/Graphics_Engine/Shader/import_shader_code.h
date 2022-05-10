#pragma once

//#include <QString>
//#include <QMessageBox>

//#include <QFile>
//#include <QTextStream>

//#include <Universal/Include/Qt_Widget_Classes/Utilities/logger_widget.h>

#include <string>
#include <fstream>

#include <Universal/Framework/FWfile_tools.h>

/*
	import_shader_code_class

	C++ class to import OpenGL glsl shader code files into the Qt QString class variable shader_string
	utilising the Qt QFile and QTextStream classes to perform this task
*/


class import_shader_code_class {
public:
	//log_widget_class       *log_widget = NULL; // pointer to log widget to report to 
	//log_display_struct_type log_display;

	//QFile       file;
	//QString     filename_to_read;
	//QTextStream stream;

	std::string  shader_string = "", input_line;
	int line_number = 0;

	//import_shader_code_class(log_widget_class *log_widget_ = NULL) {
	import_shader_code_class() {
		//log_widget = log_widget_;
	}

	// Import all the text contained in the file of pathname file_pathname_to_read into the
	// Qt QString class variable shader_string
	std::string import_code(std::string file_pathname_to_read) {
		//QMessageBox::information(NULL, "", "import_nodes HERE01\n : " + filename_to_read, QMessageBox::Ok);
		//file.setFileName(file_pathname_to_read);

		std::fstream file(file_pathname_to_read, std::ios::in);

		if (!file.is_open()) return "";
		//if (!file.open(QIODevice::ReadOnly)) return false;// Must have QIODevice::Text or endl newline not written !!

		shader_string = FW::filetools::read_all(file);

		if (shader_string == "") {
			//QMessageBox::information(0, "import_shader_code_class", "import_shader_code_class Error :\n import_shader_code_class text file \n" + file_pathname_to_read + '\n' + "is empty.", QMessageBox::Ok);
			printf("import_shader_code_class Error :\n import_shader_code_class text file \n %s \n + is empty.", file_pathname_to_read);
			//return "";
		}

		file.close();
		return shader_string;
	}

protected:

private:
};