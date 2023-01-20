#include "hex_surface_compute_generation.h"

#include <Source/Graphics_Engine/Compute/import_compute_expression_code.h>

bool hex_surface_compute_generator_class::generate_hex_surface_function() {
	if (cloud == NULL) return false;

	hex_surface_generator_parameters = cloud->hex_surface_object_data.hex_surface_generator_parameters;

	import_compute_expression_class import_compute_expression;
	//QMessageBox::information(0, "Function Expression Success", "In generate_voxel_function() :"+ voxel_generator_parameters.expression_file_pathname, QMessageBox::Ok); // testing only
	import_compute_expression.filename_to_read = hex_surface_generator_parameters.expression_file_pathname;

	if (!import_compute_expression.import_compute_expression()) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : import of file %s failed.\n", hex_surface_generator_parameters.expression_file_pathname);
		return false;
	}

	define_compute_version();
	define_work_group_invocations(hex_surface_generator_parameters.invocation);
	define_reserved_uniforms();

	define_user_uniforms();

	define_function_s();
	user_functions_s = import_compute_expression.function_code;
	define_main();
	user_main_s = import_compute_expression.expression_code;
	define_ouput();

	create_compute_shader_source_code();

	// **** test ********************************************

	//if (!define_source_code_from_file(hex_surface_generator_parameters.expression_file_pathname)) {
	//	if (log_panel != NULL) {
	//		log_panel->log_message(log_display, log_message_type_enum_type::critical, "hex_surface_compute_generator_class : generate_hex_surface_function 000 :: Could not define_source_code_from_file");
	//	}
	//	return false;
	//}

	// ********************************************

	if (!define_compute_program()) {
		if (log_panel != NULL) {
			log_panel->application_log.AddLog("CRITICAL : Hex surface : generate_hex_surface_function :: Could not create the compute shader program to generate hex surface.\n");
			log_panel->code_log.AddLog(compute_log.c_str());
		}

		return false;
	}

	if (!create_hex_surface_matrix(cloud, hex_surface_generator_parameters)) {
		if (log_panel != NULL) log_panel->application_log.AddLog("CRITICAL : Hex surface : generate_hex_surface_function :: Could not create initial hex surface grid \n");
		return false;
	}

	// It seems that for the compute shader to function and work referencing the voxel data, need to create a pointer to the QVector data 
	// as there is a problem using the QVector data() function within the opengl glBufferData and glGetBufferSubData functions
	int                    size = int(cloud->hex_surface_object_data.hex_surface_matrix_data.size());
	hex_surface_data_type* buffer = cloud->hex_surface_object_data.hex_surface_matrix_data.data();

	hex_surface_generation_execute(buffer, size, hex_surface_generator_parameters.invocation);

	// Following code block is for testing only
	/*int i = 110;
	if (log_panel != NULL) {
		log_panel->log_message(log_display, log_message_type_enum_type::info, "test_compute00 :: " + std::to_string(cloud->hex_surface_object_data.voxel_matrix_data.size()));
		//post_log_panel->log_message(log_display, log_message_type_enum_type::info, "test_compute01 :: " + std::to_string(voxel_hcp_object->hex_surface_object_data.matrix_dimension.x));
		log_panel->log_message(log_display, log_message_type_enum_type::info, "test_compute02 :: " + std::to_string(i) + " : " + std::to_string(cloud->hex_surface_object_data.voxel_matrix_data[size - 1]));

		QFile data("output.txt");
		QTextStream out(&data);
		if (!data.open(QFile::WriteOnly | QFile::Truncate)) {
			if (log_panel != NULL) {
				log_panel->log_message(log_display, log_message_type_enum_type::error, "voxel_generation :: Could not open file to ouput results to ");

			}
			//delete voxel_hcp_object;
			return false;
		}

		index_vector voxel_coord;
		QVector3D    cart_coord;
		for (i = 0; i < size; i++) {
			voxel_coord = cloud->hex_surface_object_data.get_matrix_coordinate(i);
			cart_coord = cloud->hex_surface_object_data.get_hex_cartesian_coordinate(voxel_coord, cloud->hex_surface_object_data.voxel_size);

			cart_coord = cart_coord + cloud->hex_surface_object_data.matrix_origin;

			//post_log_panel->log_message(log_display, log_message_type_enum_type::info, "test_compute01  :: i " + std::to_string(i) + "  x: " + std::to_string(voxel_coord.x) + "  y: " + std::to_string(voxel_coord.y) + "  z: " + std::to_string(voxel_coord.z) + " val: " + std::to_string(voxel_hcp_object->hex_surface_object_data.voxel_matrix_data[i]));
			out << "test_compute01  :: i " + std::to_string(i) + "  x: " + std::to_string(cart_coord.x()) + "  y: " + std::to_string(cart_coord.y()) + "  z: " + std::to_string(cart_coord.z()) + " val: " + std::to_string(cloud->hex_surface_object_data.voxel_matrix_data[i]) + '\n';
		}
	}*/

	return true;// testing
}

bool hex_surface_compute_generator_class::update_hex_surface_generation() {
	if (cloud == NULL)
		return false;

	hex_surface_generator_parameters = cloud->hex_surface_object_data.hex_surface_generator_parameters; // ****

	// It seems that for the compute shader to function and work referencing the voxel data, need to create a pointer to the QVector data 
	// as there is a problem using the QVector data() function within the opengl glBufferData and glGetBufferSubData functions
	int                    size = int(cloud->hex_surface_object_data.hex_surface_matrix_data.size());
	hex_surface_data_type* buffer = cloud->hex_surface_object_data.hex_surface_matrix_data.data();

	hex_surface_generation_execute(buffer, size, hex_surface_generator_parameters.invocation);

	return true;
}


void hex_surface_compute_generator_class::create_compute_shader_source_code() {
	source_code = "";

	source_code += version_s;
	source_code += "\n";
	source_code += "// ###### Shader inputs - ouputs #######\n";
	source_code += work_group_invocations_s;
	source_code += reserved_uniforms_s;
	source_code += "// ####################################################\n";
	source_code += "\n";
	source_code += "// +++++++++++++ Dynamicaly created user variables +++++++++++++++\n";
	source_code += "\n";
	source_code += user_uniforms_s;
	source_code += "\n";
	source_code += "// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	source_code += "\n";
	source_code += functions_s;
	source_code += "\n";
	source_code += "// ******** User defined functions to be added here ********\n";
	source_code += "\n";
	source_code += user_functions_s;
	source_code += "\n";
	source_code += "// *********************************************************\n";
	source_code += "\n";
	source_code += main_s;
	source_code += "\n";
	source_code += "// ++++++++++++++ User defined voxel output function ++++++++++++++\n";
	source_code += "\n";
	source_code += user_main_s;
	source_code += "\n";
	source_code += "// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	source_code += output_s;

	//if (log_panel != NULL) {
	//	log_panel->log_message(log_display, log_message_type_enum_type::debug, " voxel_compute_generator_class :: create_compute_shader_source_code" + source_code);
	//}
}

bool hex_surface_compute_generator_class::hex_surface_generation_execute(hex_surface_data_type* buffer, int size, int local_x_group_work_size) {
	GLuint ssbo;

	int bytes = size * sizeof(GLfloat);

	if (local_x_group_work_size > GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS) {
		std::string error_msg = "voxel_generation :: Could not execute the voxel generation computation.\n";
		error_msg = error_msg + "The specifies number of local threads  to be used exceeds the maximum\n";
		error_msg = error_msg + "permissable number of work group invocations  " + std::to_string(GL_MAX_COMPUTE_WORK_GROUP_COUNT);
		error_msg = error_msg + "Choose a smaller number for compute invocation. The minimum such number for this voxel matrix is " + std::to_string(int(ceil(float(size) / float(GL_MAX_COMPUTE_WORK_GROUP_COUNT))));
		error_msg = error_msg + "\n";

		if (log_panel != NULL)
			log_panel->application_log.AddLog(error_msg.c_str());

		return false;
	}

	int number_work_groups = int(ceil(float(size) / float(local_x_group_work_size)));

	if (number_work_groups > GL_MAX_COMPUTE_WORK_GROUP_COUNT) {
		if (log_panel != NULL) {
			std::string error_msg = "voxel_generation :: Could not execute the voxel generation computation.\n";
			error_msg = error_msg + "The specifies number of local threads to be used in the compute shader is to low for\n";
			error_msg = error_msg + "the number of data points to be processed by the GPU and causes the maximum number of permissable\n";
			error_msg = error_msg + "work groups " + std::to_string(GL_MAX_COMPUTE_WORK_GROUP_COUNT) + " to be excceded\n";
			error_msg = error_msg + "Choose a larger number for compute invocation. The minimum such number for this voxel matrix is " + std::to_string(int(ceil(float(size) / float(GL_MAX_COMPUTE_WORK_GROUP_COUNT))));
			error_msg = error_msg + "\n";

			if (log_panel != NULL)
				log_panel->application_log.AddLog(error_msg.c_str());
		}
		return false;
	}


	glUseProgram(progHandle);

	// reserve space on the GPU

	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bytes, buffer, GL_DYNAMIC_READ); // buffer data to be input into copute shader
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);                    // 0 corresponds to binding = 0 in the compute shader as a data reference

	shader.set_f1(progHandle, cloud->hex_surface_object_data.hex_surface_generator_parameters.resolution_step, "hex_size"); // this works unsigned int shader_program_id, float v, const std::string& name
	shader.set_f1(progHandle, cloud->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value, "min_surface_value"); // this works
	shader.set_f1(progHandle, cloud->hex_surface_object_data.hex_surface_generator_parameters.max_surface_value, "max_surface_value"); // this works
	shader.set_f1(progHandle, cloud->hex_surface_object_data.hex_surface_generator_parameters.e_time, "e_time"); // this works
	shader.set_i1(progHandle, cloud->hex_surface_object_data.hex_surface_generator_parameters.frame, "frame"); // this works

	shader.set_vec2(progHandle, cloud->hex_surface_object_data.grid_origin, "origin"); // this works
	shader.set_i1(progHandle, cloud->hex_surface_object_data.grid_dimension.x, "matrix_dimension_x"); // this works
	shader.set_i1(progHandle, cloud->hex_surface_object_data.grid_dimension.y, "matrix_dimension_y"); // this works
	shader.set_i1(progHandle, MIN_HEX_SURF_VALUE, "min_hex_value"); // this works
	shader.set_i1(progHandle, MAX_HEX_SURF_VALUE, "max_hex_value"); // this works
	shader.set_i1(progHandle, INVALID_HEX_SURF_VALUE, "invalid_hex_value"); // this works

	// +++++++++++ User dynamicly defined uniforms ++++++++++++++
	for (hex_surface_generator_parameter_variable_struct_type variable : hex_surface_generator_parameters.variables) {
		shader.set_f1(progHandle, variable.value, variable.variable_name);
	}

	for (hex_surface_generator_parameter_int_variable_struct_type int_variable : hex_surface_generator_parameters.int_variables) {
		shader.set_i1(progHandle, int_variable.value, int_variable.variable_name);
	}

	for (hex_surface_generator_parameter_bool_variable_struct_type bool_variable : hex_surface_generator_parameters.bool_variables) {
		shader.set_b1(progHandle, bool_variable.value, bool_variable.variable_name);
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//Tweak the geometry to the size of the buffer and its layout
	glDispatchCompute(number_work_groups, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// this gets the data from the GPU
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, bytes, buffer);          // 0 corresponds to binding = 0 in the compute shader as a data reference
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	return true;
}


bool hex_surface_compute_generator_class::create_hex_surface_matrix(hex_surface_object_class* cloud, hex_surface_generator_parameters_struct_type  hex_surface_generator_parameters) {
	cloud->hex_surface_object_data.hex_surface_matrix_data.clear();
	cloud->hex_surface_object_data.hex_surface_matrix_data.shrink_to_fit();


	// ########### CREATE EMPTY VOXEL CLOUD MATRIX #################
	float x_size = hex_surface_generator_parameters.x_end - hex_surface_generator_parameters.x_start;
	float y_size = hex_surface_generator_parameters.y_end - hex_surface_generator_parameters.y_start;

	float x_res_step = hex_surface_generator_parameters.resolution_step * 2.0f;
	float y_res_step = hex_surface_generator_parameters.resolution_step * (3.0f / sqrt(3.0f));

	int data_set_x_size, data_set_y_size;

	if (x_size / x_res_step - float((int)(x_size / x_res_step)) > 0.0f)
		data_set_x_size = (int)(x_size / x_res_step) + 1;
	else
		data_set_x_size = (int)(x_size / x_res_step);

	if (y_size / y_res_step - float((int)(y_size / y_res_step)) > 0.0f)
		data_set_y_size = (int)(y_size / y_res_step) + 1;
	else
		data_set_y_size = (int)(y_size / y_res_step);
	//QMessageBox::information(0, "Function Expression Success", "create_voxel_matrix 00: "+std::to_string(data_set_x_size)+":"+std::to_string(data_set_y_size)+":"+std::to_string(data_set_z_size)+":", QMessageBox::Ok);

	glm::vec2 origin = { hex_surface_generator_parameters.x_start,hex_surface_generator_parameters.y_start };

	cloud->hex_surface_object_data.hex_size = hex_surface_generator_parameters.resolution_step;

	cloud->hex_surface_object_data.grid_dimension = { data_set_x_size,data_set_y_size,0 };
	cloud->hex_surface_object_data.grid_origin = origin;
	cloud->hex_surface_object_data.create_empty_surface_cubic(data_set_x_size, data_set_y_size);
	//QMessageBox::information(0, "Function Expression Success", "create_voxel_matrix 01: "+std::to_string(cloud->hex_surface_object_data.voxel_matrix_data.size())+":", QMessageBox::Ok);

	if (cloud->hex_surface_object_data.hex_surface_matrix_data.size() > 0)
		return true;
	else
		return false;
}

//----------------------------------------------

void hex_surface_compute_generator_class::define_compute_version() {
	if (ogl_version == compute_ogl_version_enum::v45)
		version_s = "#version 450\n";
	else
		version_s = "#version 430\n";
}

void hex_surface_compute_generator_class::define_work_group_invocations(int invocations) {
	work_group_invocations_s = "layout(local_size_x = " + std::to_string(invocations) + ", local_size_y = 1, local_size_z = 1) in;\n";
}

void hex_surface_compute_generator_class::define_reserved_uniforms() {
	reserved_uniforms_s = "// only std430 will work\n";
	reserved_uniforms_s += "layout(std430, binding = 0) buffer ot { float buffer_out[]; };\n";
	reserved_uniforms_s += "\n";
	reserved_uniforms_s += "layout(location = 1) uniform float hex_size;\n";

	reserved_uniforms_s += "layout(location = 2) uniform int min_surface_value;\n";
	reserved_uniforms_s += "layout(location = 3) uniform int max_surface_value;\n";
	reserved_uniforms_s += "layout(location = 4) uniform float e_time;\n";
	reserved_uniforms_s += "layout(location = 5) uniform int frame;\n";
	reserved_uniforms_s += "\n";
	reserved_uniforms_s += "layout(location = 6) uniform vec2  origin;\n";
	reserved_uniforms_s += "layout(location = 7) uniform int matrix_dimension_x;\n";
	reserved_uniforms_s += "layout(location = 8) uniform int matrix_dimension_y;\n";

	reserved_uniforms_s += "layout(location = 9)  uniform int min_hex_value;\n";
	reserved_uniforms_s += "layout(location = 10) uniform int max_hex_value;\n";
	reserved_uniforms_s += "layout(location = 11) uniform int invalid_hex_value;\n";
}

void hex_surface_compute_generator_class::define_user_uniforms() {
	user_uniforms_s = "";
	int location = 14; // This must be one greater than the max layout location of the reserved uniforms in define_reserved_uniforms()

	for (hex_surface_generator_parameter_variable_struct_type variable : hex_surface_generator_parameters.variables) {
		user_uniforms_s += "layout(location = " + std::to_string(location) + ") uniform float " + variable.variable_name + ";\n"; // Need to introduce capability to cater for int and and boolean types
		location++;
	}

	for (hex_surface_generator_parameter_int_variable_struct_type int_variable : hex_surface_generator_parameters.int_variables) {
		user_uniforms_s += "layout(location = " + std::to_string(location) + ") uniform int " + int_variable.variable_name + ";\n"; // Need to introduce capability to cater for int and and boolean types
		location++;
	}

	for (hex_surface_generator_parameter_bool_variable_struct_type bool_variable : hex_surface_generator_parameters.bool_variables) {
		user_uniforms_s += "layout(location = " + std::to_string(location) + ") uniform int " + bool_variable.variable_name + ";\n"; // Need to introduce capability to cater for int and and boolean types
		location++;
	}
}

void hex_surface_compute_generator_class::define_function_s() {
	functions_s = "// ========= Voxel generation data storage functions =============\n";
	functions_s += "\n";
	functions_s += "uvec2 matrix_dimension;\n";
	functions_s += "\n";
	functions_s += "struct index_vector {\n";
	functions_s += "   uint x, y;\n";
	functions_s += "};\n";
	functions_s += "\n";
	functions_s += "// Retreive the 3 dim x,y coordinate that the matrix index corresponds to\n";
	functions_s += "index_vector get_matrix_coordinate(uint matrix_index) {\n";
	functions_s += "	index_vector matrix_coord;\n";
	functions_s += "\n";
	functions_s += "// Get the array matrix index value of the x=0, y=0 z= matrix_coord.z voxel coordinate\n";
	functions_s += "uint zeroth_index = matrix_index;\n";
	functions_s += "uint yt = 2 * matrix_dimension.x - 1; // Even row number of voxels + odd row of voxels of any z level\n";
	functions_s += "\n";
	functions_s += "uint yt_index01, yt_index02;\n";
	functions_s += "\n";
	functions_s += "yt_index01 = int(zeroth_index / yt);\n";
	functions_s += "\n";
	functions_s += "if (((zeroth_index - yt_index01 * yt) % yt) < matrix_dimension.x)\n";
	functions_s += "	yt_index02 = 0;\n";
	functions_s += "else\n";
	functions_s += "	yt_index02 = 1;\n";
	functions_s += "\n";
	functions_s += "	matrix_coord.y = yt_index01 * 2 + yt_index02;\n";
	functions_s += "	matrix_coord.x = zeroth_index - ((int(matrix_coord.y / 2) + matrix_coord.y % 2) * matrix_dimension.x + int(matrix_coord.y / 2) * (matrix_dimension.x - 1));\n";
	functions_s += "\n";
	functions_s += "	return matrix_coord;\n";
	functions_s += "}\n";
	functions_s += "\n";
	functions_s += "vec3 get_hex_cartesian_coordinate(index_vector voxel_coord, float hex_size) {\n";
	functions_s += "	uint i = voxel_coord.x, j = voxel_coord.y, k = 0;\n";
	functions_s += "\n";
	functions_s += "	float sqrt3 = sqrt(3.0), third = 1.0 / 3.0, z_mult = 2.0 * sqrt(6.0) / 3.0, sqrt3_2 = sqrt(1.5);\n";
	functions_s += "	vec3 hex_cartesian_coordinate;\n";
	functions_s += "\n";
	functions_s += "	if (j % 2 == 0) {\n";
	functions_s += "		hex_cartesian_coordinate.x = ((float(i) * 2 + float(k % 2)) * hex_size);\n";
	functions_s += "		hex_cartesian_coordinate.y = ((sqrt3 * float(j) + sqrt3 * third * float(k % 2)) * hex_size);\n";
	functions_s += "	}\n";
	functions_s += "	else {\n";
	functions_s += "		hex_cartesian_coordinate.x = ((-1.0 + float(i) * 2 + float(k % 2) + 2 * float((k + 1) % 2)) * hex_size);\n";
	functions_s += "		hex_cartesian_coordinate.y = ((sqrt3 + sqrt3 * (float(j) - 1) + sqrt3 * third * float(k % 2)) * hex_size);\n";
	functions_s += "	}\n";
	functions_s += "\n";
	functions_s += "	hex_cartesian_coordinate.z = 0.0;\n";
	functions_s += "\n";
	functions_s += "	return hex_cartesian_coordinate;\n";
	functions_s += "}\n";
	functions_s += "\n";
	functions_s += "vec3 get_hex_cartesian_coordinate(index_vector voxel_coord) {\n";
	functions_s += "	return get_hex_cartesian_coordinate(voxel_coord, hex_size);\n";
	functions_s += "}\n";
}

void hex_surface_compute_generator_class::define_main() {
	main_s = "void main() {\n";
	main_s += "    uint matrix_index = gl_GlobalInvocationID.x;\n";
	main_s += "\n";
	main_s += "    float output_result = 0; // variable that must be used to output the function result\n";
	main_s += "\n";
	main_s += "    matrix_dimension.x = matrix_dimension_x;\n";
	main_s += "    matrix_dimension.y = matrix_dimension_y;\n";
	main_s += "\n";
	main_s += "    // get the cartesian coordinate that correspond to the matrix_index value\n";
	main_s += "    index_vector hex_coord = get_matrix_coordinate(matrix_index);\n";
	main_s += "    vec3 matrix_cart_coord = get_hex_cartesian_coordinate(hex_coord, hex_size);\n";
	main_s += "\n";
	main_s += "    float x = matrix_cart_coord.x + origin.x;\n";
	main_s += "    float y = matrix_cart_coord.y + origin.y;\n";
}

void hex_surface_compute_generator_class::define_ouput() {
	output_s = "    buffer_out[matrix_index] = output_result; // output the voxel volume result\n";
	output_s += "}\n";
}

