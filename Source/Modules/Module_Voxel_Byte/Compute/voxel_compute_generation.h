#pragma once

#include <Graphics_Engine/Compute/compute_shader.h>
#include <Graphics_Engine/Compute/import_compute_expression_code.h>

#include "../Voxel_hcp_object/DataTypes/dt_voxel_generator.h"
#include "../Voxel_hcp_object/voxel_hcp_object.h"

#include <Graphics_Engine/Shader/shader.h>

/*
							voxel_compute_generator_class

	C++ class that generates by use of a glsl compute shader program the hcp voxel point cloud
	data and stores the results in the hcp voxel class point cloud data matrix.

	The OpenGL glsl compute shader program is created from several source text files.

	The source code is split into specific text files for different sections of the glsl program
	so as to create the ability for the application to dynamicly allow the glsl program to accomodate
	interactive definitions and assignments of glsl shader variables. This was also done for the user
	to write custom code targeted to be used in the glsl programs main function.

	This class imports all the source code text files of each of these sections and them combines them
	into a single Qt QString class storage variable ready for adding to a OpenGL shader program for
	compilation
*/

class voxel_compute_generator_class : public compute_shader_class {
public:
	voxel_hcp_object_class* cloud = NULL;// pointer to the hcp voxel class in the virtual worlds scene data model

	voxel_generator_parameters_struct_type voxel_generator_parameters; //****

	bool generate_voxel_function() {
		if (cloud == NULL) return false;

		voxel_generator_parameters = cloud->voxel_object_data.voxel_generator_parameters;

		import_compute_expression_class import_compute_expression;
		//QMessageBox::information(0, "Function Expression Success", "In generate_voxel_function() :"+ voxel_generator_parameters.expression_file_pathname, QMessageBox::Ok); // testing only
		import_compute_expression.filename_to_read = voxel_generator_parameters.expression_file_pathname;

		if (!import_compute_expression.import_compute_expression()) {
			std::string message = "import_exprtk_expression of file " + voxel_generator_parameters.expression_file_pathname + " failed.\n";
			printf(message.c_str());
			return false;
		}

		define_compute_version();
		define_work_group_invocations(voxel_generator_parameters.invocation);
		define_reserved_uniforms();

		define_user_uniforms();

		define_function_s();
		user_functions_s = import_compute_expression.function_code;
		define_main();
		user_main_s = import_compute_expression.expression_code;
		define_ouput();


		create_compute_shader_source_code();

		if (!define_compute_program()) {
			printf("critical, voxel_generation : generate_voxel_function::Could not create the compute shader program to generate voxel matrix.\n");
			return false;
		}

		if (!create_voxel_matrix(cloud, voxel_generator_parameters)) {
			printf("critical, voxel_generation : generate_voxel_function::Could not create initial voxel data matrix.\n");
			return false;
		}

		// It seems that for the compute shader to function and work referencing the voxel data, need to create a pointer to the QVector data 
		// as there is a problem using the QVector data() function within the opengl glBufferData and glGetBufferSubData functions
		int size = cloud->voxel_object_data.voxel_matrix_data.size();
//for(int i = 0; i< size;i++){
//	printf("generate_voxel_function 0000 : %i \n", cloud->voxel_object_data.voxel_matrix_data[i]);
//}
		voxel_data_type* buffer = cloud->voxel_object_data.voxel_matrix_data.data();
		voxel_generation_execute(buffer, size, voxel_generator_parameters.invocation);


//for(int i = 0; i< size;i++){
//	printf("generate_voxel_function 1111 : %i \n", cloud->voxel_object_data.voxel_matrix_data[i]);
//}

// Following code block is for testing only
/*int i = 110;
if (log_widget != NULL) {
	log_widget->log_message(log_display, log_message_type_enum_type::info, "test_compute00 :: " + QString::number(cloud->voxel_object_data.voxel_matrix_data.size()));
	//post_log_widget->log_message(log_display, log_message_type_enum_type::info, "test_compute01 :: " + QString::number(voxel_hcp_object->voxel_object_data.matrix_dimension.x));
	log_widget->log_message(log_display, log_message_type_enum_type::info, "test_compute02 :: " + QString::number(i) + " : " + QString::number(cloud->voxel_object_data.voxel_matrix_data[size - 1]));

	QFile data("output.txt");
	QTextStream out(&data);
	if (!data.open(QFile::WriteOnly | QFile::Truncate)) {
		if (log_widget != NULL) {
			log_widget->log_message(log_display, log_message_type_enum_type::error, "voxel_generation :: Could not open file to ouput results to ");

		}
		//delete voxel_hcp_object;
		return false;
	}

	index_vector voxel_coord;
	QVector3D    cart_coord;
	for (i = 0; i < size; i++) {
		voxel_coord = cloud->voxel_object_data.get_matrix_coordinate(i);
		cart_coord = cloud->voxel_object_data.get_voxel_cartesian_coordinate(voxel_coord, cloud->voxel_object_data.voxel_size);

		cart_coord = cart_coord + cloud->voxel_object_data.matrix_origin;

		//post_log_widget->log_message(log_display, log_message_type_enum_type::info, "test_compute01  :: i " + QString::number(i) + "  x: " + QString::number(voxel_coord.x) + "  y: " + QString::number(voxel_coord.y) + "  z: " + QString::number(voxel_coord.z) + " val: " + QString::number(voxel_hcp_object->voxel_object_data.voxel_matrix_data[i]));
		out << "test_compute01  :: i " + QString::number(i) + "  x: " + QString::number(cart_coord.x()) + "  y: " + QString::number(cart_coord.y()) + "  z: " + QString::number(cart_coord.z()) + " val: " + QString::number(cloud->voxel_object_data.voxel_matrix_data[i]) + '\n';
	}
}*/

		return true;// testing
	}

	bool update_voxel_generation() {
		if (cloud == NULL)
			return false;

		voxel_generator_parameters = cloud->voxel_object_data.voxel_generator_parameters; // ****

		// It seems that for the compute shader to function and work referencing the voxel data, need to create a pointer to the QVector data 
		// as there is a problem using the QVector data() function within the opengl glBufferData and glGetBufferSubData functions
		int              size = cloud->voxel_object_data.voxel_matrix_data.size();
		voxel_data_type* buffer = cloud->voxel_object_data.voxel_matrix_data.data();

		voxel_generation_execute(buffer, size, voxel_generator_parameters.invocation);

		return true;
	}

	bool voxel_generation_execute(voxel_data_type* buffer, int size, int local_x_group_work_size) {
		GLuint ssbo;

		int bytes = size * sizeof(GLuint);

		if (local_x_group_work_size > GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS) {
			std::string error_msg = "voxel_generation :: Could not ececute the voxel generation computation.\n";
			error_msg = error_msg + "The specifies number of local threads local_size_x to be used exceeds the maximum\n";
			error_msg = error_msg + "permissable number of work group invocations  " + std::to_string(GL_MAX_COMPUTE_WORK_GROUP_COUNT);
			error_msg = error_msg + "Choose a smaller number for local_size_x. The minimum such number for this voxel matrix is " + std::to_string(int(ceil(float(size) / float(GL_MAX_COMPUTE_WORK_GROUP_COUNT))));

			//log_widget->log_message(log_display, log_message_type_enum_type::critical, error_msg);
			printf("Critical : %s\n", error_msg.c_str());
		}

		int number_work_groups = int(ceil(float(size) / float(local_x_group_work_size)));

		if (number_work_groups > GL_MAX_COMPUTE_WORK_GROUP_COUNT) {
			std::string error_msg = "voxel_generation :: Could not ececute the voxel generation computation.\n";
			error_msg = error_msg + "The specifies number of local threads local_size_x to be used in the compute shader is to low for\n";
			error_msg = error_msg + "the number of data points to be processed by the GPU and causes the maximum number of permissable\n";
			error_msg = error_msg + "work groups " + std::to_string(GL_MAX_COMPUTE_WORK_GROUP_COUNT) + " to be excceded\n";
			error_msg = error_msg + "Choose a larger number for local_size_x. The minimum such number for this voxel matrix is " + std::to_string(int(ceil(float(size) / float(GL_MAX_COMPUTE_WORK_GROUP_COUNT))));

			//log_widget->log_message(log_display, log_message_type_enum_type::critical, error_msg);
			printf("Critical : %s\n", error_msg.c_str());
			return false;
		}

		glUseProgram(progHandle);

		glGenBuffers(1, &ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, bytes, buffer, GL_DYNAMIC_READ); // buffer data to be input into copute shader

		//f->glBufferData(GL_SHADER_STORAGE_BUFFER, bytes, voxel_matrix_data.data(), GL_DYNAMIC_READ); // this method does not work
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);                    // 0 corresponds to binding = 0 in the compute shader as a data reference

		shader.set_f1(progHandle, cloud->voxel_object_data.voxel_size, "voxel_size"); // this works unsigned int shader_program_id, float v, const std::string& name
		shader.set_f1(progHandle, cloud->voxel_object_data.voxel_generator_parameters.generation_threshold, "threshold"); // this works
		shader.set_f1(progHandle, cloud->voxel_object_data.voxel_generator_parameters.min_surface_value, "min_surface_value"); // this works
		shader.set_f1(progHandle, cloud->voxel_object_data.voxel_generator_parameters.max_surface_value, "max_surface_value"); // this works
		shader.set_f1(progHandle, cloud->voxel_object_data.voxel_generator_parameters.e_time, "e_time"); // this works
		shader.set_i1(progHandle, cloud->voxel_object_data.voxel_generator_parameters.frame, "frame"); // this works

		shader.set_vec3(progHandle, cloud->voxel_object_data.matrix_origin, "origin"); // this works
		shader.set_i1(progHandle, cloud->voxel_object_data.matrix_dimension.x, "matrix_dimension_x"); // this works
		shader.set_i1(progHandle, cloud->voxel_object_data.matrix_dimension.y, "matrix_dimension_y"); // this works
		shader.set_i1(progHandle, cloud->voxel_object_data.matrix_dimension.z, "matrix_dimension_z"); // this works
		shader.set_i1(progHandle, MIN_VOXEL_VALUE, "min_voxel_value"); // this works
		shader.set_i1(progHandle, MAX_VOXEL_VALUE, "max_voxel_value"); // this works
		shader.set_i1(progHandle, INVALID_VOXEL_VALUE, "invalid_voxel_value"); // this works

		// +++++++++++ User dynamicly defined uniforms ++++++++++++++
		for (voxel_generator_parameter_variable_struct_type variable : voxel_generator_parameters.variables) {
			shader.set_f1(progHandle, variable.value, variable.variable_name);
		}

		for (voxel_generator_parameter_int_variable_struct_type int_variable : voxel_generator_parameters.int_variables) {
			shader.set_i1(progHandle, int_variable.value, int_variable.variable_name);
		}

		for (voxel_generator_parameter_bool_variable_struct_type bool_variable : voxel_generator_parameters.bool_variables) {
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

	bool create_voxel_matrix(voxel_hcp_object_class* cloud, voxel_generator_parameters_struct_type  voxel_generator_parameters) {
		cloud->voxel_object_data.voxel_matrix_data.clear();   // ******
		cloud->voxel_object_data.voxel_matrix_data.shrink_to_fit(); // ******


		// ########### CREATE EMPTY VOXEL CLOUD MATRIX #################
		float x_size = voxel_generator_parameters.x_end - voxel_generator_parameters.x_start;
		float y_size = voxel_generator_parameters.y_end - voxel_generator_parameters.y_start;
		float z_size = voxel_generator_parameters.z_end - voxel_generator_parameters.z_start;
		float z_mult = 2.0 * sqrt(6.0) / 3.0;

		float x_res_step = voxel_generator_parameters.resolution_step * 2.0;
		float y_res_step = voxel_generator_parameters.resolution_step * (3.0 / sqrt(3.0));
		float z_res_step = voxel_generator_parameters.resolution_step * z_mult;

		int data_set_x_size, data_set_y_size, data_set_z_size;

		if (x_size / x_res_step - float((int)(x_size / x_res_step)) > 0.0)
			data_set_x_size = (int)(x_size / x_res_step) + 1;
		else
			data_set_x_size = (int)(x_size / x_res_step);

		if (y_size / y_res_step - float((int)(y_size / y_res_step)) > 0.0)
			data_set_y_size = (int)(y_size / y_res_step) + 1;
		else
			data_set_y_size = (int)(y_size / y_res_step);

		if (z_size / z_res_step - float((int)(z_size / z_res_step)) > 0.0)
			data_set_z_size = (int)(z_size / z_res_step) + 1;
		else
			data_set_z_size = (int)(z_size / z_res_step);
		//QMessageBox::information(0, "Function Expression Success", "create_voxel_matrix 00: "+QString::number(data_set_x_size)+":"+QString::number(data_set_y_size)+":"+QString::number(data_set_z_size)+":", QMessageBox::Ok);

		glm::vec3 origin = { voxel_generator_parameters.x_start,voxel_generator_parameters.y_start,voxel_generator_parameters.z_start };

		cloud->voxel_object_data.voxel_size = voxel_generator_parameters.resolution_step;

		cloud->voxel_object_data.matrix_dimension = { data_set_x_size,data_set_y_size,data_set_z_size };
		cloud->voxel_object_data.matrix_origin = origin;
		cloud->voxel_object_data.create_empty_volume_cubic(data_set_x_size, data_set_y_size, data_set_z_size);
		//QMessageBox::information(0, "Function Expression Success", "create_voxel_matrix 01: "+QString::number(cloud->voxel_object_data.voxel_matrix_data.size())+":", QMessageBox::Ok);

		if (cloud->voxel_object_data.voxel_matrix_data.size() > 0)
			return true;
		else
			return false;
	}

	void create_compute_shader_source_code() {
		source_code = "";

		//source_code += version_s;
		source_code += "#version 450 core\n";
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

		//if (log_widget != NULL) {
		//	log_widget->log_message(log_display, log_message_type_enum_type::debug, " voxel_compute_generator_class :: create_compute_shader_source_code" + source_code);
		//}
	}

private:
	shader_class shader;

	std::vector <voxel_generator_function_variable_struct_type> function_variables;

	std::string version_s, work_group_invocations_s, reserved_uniforms_s, functions_s, main_s, output_s;
	std::string user_uniforms_s, user_functions_s, user_main_s;

	void define_compute_version() {
		if (ogl_version == compute_ogl_version_enum::v45)
			version_s = "#version 450\n";
		else
			version_s = "#version 430\n";
	}

	void define_work_group_invocations(int invocations) {
		work_group_invocations_s = "layout(local_size_x = " + std::to_string(invocations) + ", local_size_y = 1, local_size_z = 1) in;\n";
	}

	void define_reserved_uniforms() {
		reserved_uniforms_s = "// only std430 will work\n";
		reserved_uniforms_s += "layout(std430, binding = 0) buffer ot { uint buffer_out[]; };\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "layout(location = 1) uniform float voxel_size;\n";
		reserved_uniforms_s += "layout(location = 2) uniform float threshold;\n";
		reserved_uniforms_s += "layout(location = 3) uniform int min_surface_value;\n";
		reserved_uniforms_s += "layout(location = 4) uniform int max_surface_value;\n";
		reserved_uniforms_s += "layout(location = 5) uniform float e_time;\n";
		reserved_uniforms_s += "layout(location = 6) uniform int frame;\n";
		reserved_uniforms_s += "\n";
		reserved_uniforms_s += "layout(location = 7) uniform vec3  origin;\n";
		reserved_uniforms_s += "layout(location = 8) uniform int matrix_dimension_x;\n";
		reserved_uniforms_s += "layout(location = 9) uniform int matrix_dimension_y;\n";
		reserved_uniforms_s += "layout(location = 10) uniform int matrix_dimension_z;\n";
		reserved_uniforms_s += "layout(location = 11) uniform int min_voxel_value;\n";
		reserved_uniforms_s += "layout(location = 12) uniform int max_voxel_value;\n";
		reserved_uniforms_s += "layout(location = 13) uniform int invalid_voxel_value;\n";
	}

	void  define_user_uniforms() {
		user_uniforms_s = "";
		int location = 14; // This must be one greater than the max layout location of the reserved uniforms in define_reserved_uniforms()

		for (voxel_generator_parameter_variable_struct_type variable : voxel_generator_parameters.variables) {
			user_uniforms_s += "layout(location = " + std::to_string(location) + ") uniform float " + variable.variable_name.c_str() + ";\n"; // Need to introduce capability to cater for int and and boolean types
			location++;
		}

		for (voxel_generator_parameter_int_variable_struct_type int_variable : voxel_generator_parameters.int_variables) {
			user_uniforms_s += "layout(location = " + std::to_string(location) + ") uniform int " + int_variable.variable_name.c_str() + ";\n"; // Need to introduce capability to cater for int and and boolean types
			location++;
		}

		for (voxel_generator_parameter_bool_variable_struct_type bool_variable : voxel_generator_parameters.bool_variables) {
			user_uniforms_s += "layout(location = " + std::to_string(location) + ") uniform int " + bool_variable.variable_name.c_str() + ";\n"; // Need to introduce capability to cater for int and and boolean types
			location++;
		}
	}

	void define_function_s() {
		functions_s = "// ========= Voxel generation data storage functions =============\n";
		functions_s += "\n";
		functions_s += "uvec3 matrix_dimension;\n";
		functions_s += "\n";
		functions_s += "struct index_vector {\n";
		functions_s += "   uint x, y, z;\n";
		functions_s += "};\n";
		functions_s += "\n";
		functions_s += "uint get_z_layer_index_value(uint iX, uint iY, uint iZ) {\n";
		functions_s += "if (iZ % 2 == 0) // Even z level\n";
		functions_s += "    return (iY / 2 + iY % 2) * matrix_dimension.x + iY / 2 * (matrix_dimension.x - 1) + iX;\n";
		functions_s += "else // Odd z level\n";
		functions_s += "    return (iY / 2 + iY % 2) * (matrix_dimension.x - 1) + iY / 2 * matrix_dimension.x + iX;\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "uint get_z_layer_total2(uint iZ, uint xdim, uint ydim) {\n";
		functions_s += "if (iZ % 2 == 0) // Even z level\n";
		functions_s += "    return (int(ydim / 2) + ydim % 2) * xdim + int(ydim / 2) * (xdim - 1);\n";
		functions_s += "else // Odd z level\n";
		functions_s += "    return (int((ydim - 1) / 2) + (ydim - 1) % 2) * (xdim - 1) + int((ydim - 1) / 2) * xdim;\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "uint get_z_layer_total(uint iZ) {\n";
		functions_s += "    return get_z_layer_total2(iZ, matrix_dimension.x, matrix_dimension.y);\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "uint get_index_value(uint iX, uint iY, uint iZ) {\n";
		functions_s += "    return (iZ / 2 + iZ % 2) * get_z_layer_total(0) + iZ / 2 * get_z_layer_total(1) + get_z_layer_index_value(iX, iY, iZ);\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "// Retreive the 3 dim x,y,z coordinate that the matrix index corresponds to\n";
		functions_s += "index_vector get_matrix_coordinate(uint matrix_index) {\n";
		functions_s += "index_vector matrix_coord;\n";
		functions_s += "\n";
		functions_s += "uint t0 = get_z_layer_total(0);// total number for even z level\n";
		functions_s += "uint t1 = get_z_layer_total(1);// total number for odd  z level\n";
		functions_s += "\n";
		functions_s += "uint zt = t0 + t1; // total number for even + odd z levels\n";
		functions_s += "\n";
		functions_s += "bool even_z_level;\n";
		functions_s += "\n";
		functions_s += "if (matrix_index < t0) {\n";
		functions_s += "    matrix_coord.z = 0;\n";
		functions_s += "    even_z_level = true;\n";
		functions_s += "}\n";
		functions_s += "else {\n";
		functions_s += "    int zt_index01 = int(matrix_index / zt);\n";
		functions_s += "    int zt_index02 = int((matrix_index - zt_index01 * zt) / t0);\n";
		functions_s += "\n";
		functions_s += "    if (zt_index02 > 0) { // Have matrix_index in the odd  z level\n";
		functions_s += "        matrix_coord.z = zt_index01 * 2 + 1;\n";
		functions_s += "        even_z_level = false;\n";
		functions_s += "    }\n";
		functions_s += "    else {// Have matrix_index in the even z level\n";
		functions_s += "        matrix_coord.z = zt_index01 * 2;\n";
		functions_s += "        even_z_level = true;\n";
		functions_s += "    }\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "// Get the array matrix index value of the x=0, y=0 z= matrix_coord.z voxel coordinate\n";
		functions_s += "uint zeroth_index = matrix_index - get_index_value(0, 0, matrix_coord.z);\n";
		functions_s += "uint yt = 2 * matrix_dimension.x - 1; // Even row number of voxels + odd row of voxels of any z level\n";
		functions_s += "\n";
		functions_s += "if (even_z_level) {\n";
		functions_s += "    uint yt_index01, yt_index02;\n";
		functions_s += "\n";
		functions_s += "    yt_index01 = int(zeroth_index / yt);\n";
		functions_s += "\n";
		functions_s += "    if (((zeroth_index - yt_index01 * yt) % yt) < matrix_dimension.x)\n";
		functions_s += "        yt_index02 = 0;\n";
		functions_s += "    else\n";
		functions_s += "        yt_index02 = 1;\n";
		functions_s += "\n";
		functions_s += "    matrix_coord.y = yt_index01 * 2 + yt_index02;\n";
		functions_s += "    matrix_coord.x = zeroth_index - ((int(matrix_coord.y / 2) + matrix_coord.y % 2) * matrix_dimension.x + int(matrix_coord.y / 2) * (matrix_dimension.x - 1));\n";
		functions_s += "}else {\n";
		functions_s += "    uint yt_index01, yt_index02;\n";
		functions_s += "\n";
		functions_s += "    yt_index01 = int(zeroth_index / yt);\n";
		functions_s += "\n";
		functions_s += "    if (((zeroth_index - yt_index01 * yt) % yt) < matrix_dimension.x - 1)\n";
		functions_s += "         yt_index02 = 0;\n";
		functions_s += "    else\n";
		functions_s += "         yt_index02 = 1;\n";
		functions_s += "\n";
		functions_s += "    matrix_coord.y = yt_index01 * 2 + yt_index02;\n";
		functions_s += "    matrix_coord.x = zeroth_index - ((int(matrix_coord.y / 2) + matrix_coord.y % 2) * (matrix_dimension.x - 1) + int(matrix_coord.y / 2) * matrix_dimension.x);\n";
		functions_s += "}\n";
		functions_s += "return matrix_coord;\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "vec3 get_voxel_cartesian_coordinate(index_vector voxel_coord, float voxel_size) {\n";
		functions_s += "uint i = voxel_coord.x, j = voxel_coord.y, k = voxel_coord.z;\n";
		functions_s += "\n";
		functions_s += "float sqrt3 = sqrt(3.0), third = 1.0 / 3.0, z_mult = 2.0 * sqrt(6.0) / 3.0, sqrt3_2 = sqrt(1.5);\n";
		functions_s += "vec3 voxel_cartesian_coordinate;\n";
		functions_s += "\n";
		functions_s += "if (j % 2 == 0) {\n";
		functions_s += "    voxel_cartesian_coordinate.x = ((float(i) * 2 + float(k % 2)) * voxel_size);\n";
		functions_s += "    voxel_cartesian_coordinate.y = ((sqrt3 * float(j) + sqrt3 * third * float(k % 2)) * voxel_size);\n";
		functions_s += "} else {\n";
		functions_s += "    voxel_cartesian_coordinate.x = ((-1.0 + float(i) * 2 + float(k % 2) + 2 * float((k + 1) % 2)) * voxel_size);\n";
		functions_s += "    voxel_cartesian_coordinate.y = ((sqrt3 + sqrt3 * (float(j) - 1) + sqrt3 * third * float(k % 2)) * voxel_size);\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "voxel_cartesian_coordinate.z = (z_mult * float(k) * voxel_size);\n";
		functions_s += "\n";
		functions_s += "return voxel_cartesian_coordinate;\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "vec3 get_voxel_cartesian_coordinate(index_vector voxel_coord) {\n";
		functions_s += "    return get_voxel_cartesian_coordinate(voxel_coord, voxel_size);\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "uint validate_result_value(uint output_result) {\n";
		functions_s += "	uint result;\n";
		functions_s += "	if (output_result > max_surface_value)\n";
		functions_s += "		result = invalid_voxel_value;\n";
		functions_s += "	else {\n";
		functions_s += "		if (output_result < min_surface_value)\n";
		functions_s += "			result = invalid_voxel_value;\n";
		functions_s += "		else {\n";
		functions_s += "			result = uint(float(output_result - min_surface_value) * (float(max_voxel_value) / float(max_surface_value - min_surface_value)));\n";
		functions_s += "			if (result < min_voxel_value)\n";
		functions_s += "				result = min_voxel_value;\n";
		functions_s += "			else\n";
		functions_s += "				if (result > max_voxel_value)\n";
		functions_s += "					result = max_voxel_value;\n";
		functions_s += "		}\n";
		functions_s += "	}\n";
		functions_s += "	return result;\n";
		functions_s += "}\n";
		functions_s += "\n";
		functions_s += "uint validate_color_element_value(uint color_value) {\n";
		functions_s += "	uint color;\n";
		functions_s += "	if (color_value > 255)\n";
		functions_s += "		color = 255;\n";
		functions_s += "	else\n";
		functions_s += "		color = color_value;\n";
		functions_s += "\n";
		functions_s += "	return color;\n";
		functions_s += "}\n";
	}

	void define_main() {
		main_s = "void main() {\n";
		main_s += "    uint matrix_index = gl_GlobalInvocationID.x;\n";
		main_s += "\n";
		main_s += "    uint output_result = 0; // variable that must be used to output the function result\n";
		main_s += "\n";
		main_s += "    matrix_dimension.x = matrix_dimension_x;\n";
		main_s += "    matrix_dimension.y = matrix_dimension_y;\n";
		main_s += "    matrix_dimension.z = matrix_dimension_z;\n";
		main_s += "\n";
		main_s += "    // get the cartesian coordinate that correspond to the matrix_index value\n";
		main_s += "    index_vector voxel_coord = get_matrix_coordinate(matrix_index);\n";
		main_s += "    vec3 matrix_cart_coord = get_voxel_cartesian_coordinate(voxel_coord, voxel_size);\n";
		main_s += "\n";
		main_s += "    float x = matrix_cart_coord.x + origin.x;\n";
		main_s += "    float y = matrix_cart_coord.y + origin.y;\n";
		main_s += "    float z = matrix_cart_coord.z + origin.z;\n";
		main_s += "\n";
		//main_s += "    uint red = 255, green = 255, blue = 255, value = 0;\n"; // Will no longer use this 
		main_s += "    uint value = 0;\n"; // Will no longer use this 
	}

	void define_ouput() {
		output_s = "\n";
		//	output_s =  "red   = validate_color_element_value(red);\n";
		//	output_s += "green = validate_color_element_value(green);\n";
		//	output_s += "blue  = validate_color_element_value(blue);\n";
		//	output_s += "value = validate_result_value(value);\n";
		//	output_s += "\n";
		//	output_s += "output_result = red << 24 | green << 16 | blue << 8 | value;\n";
		//	output_s += "\n";
		//	output_s += "    buffer_out[matrix_index] = output_result; // output the voxel volume result\n";
		output_s += "    buffer_out[matrix_index] = value; // output the voxel volume result\n";
		output_s += "}\n";
	}



};