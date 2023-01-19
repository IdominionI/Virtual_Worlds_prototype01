#include "hex_surface_object.h"

void hex_surface_object_class::clear_shader_variables() {
	hex_surface_object_data.hex_surface_shader_parameters.variables.clear();
	hex_surface_object_data.hex_surface_shader_parameters.variables.shrink_to_fit();
	hex_surface_object_data.hex_surface_shader_parameters.int_variables.clear();
	hex_surface_object_data.hex_surface_shader_parameters.int_variables.shrink_to_fit();
	hex_surface_object_data.hex_surface_shader_parameters.bool_variables.clear();
	hex_surface_object_data.hex_surface_shader_parameters.bool_variables.shrink_to_fit();
	// other variables added below
}

bool hex_surface_object_class::automata_rule_within_frame(int frame) {
	bool is_within_animation_frame = false;
	for (hex_surface_automata_rule_struct_type autmata_rule : hex_surface_object_data.hex_surface_automata_rules) {
		if (frame >= autmata_rule.start_step && frame <= autmata_rule.end_step) is_within_animation_frame = true;
	}

	return is_within_animation_frame;
}

bool hex_surface_object_class::define_vbo_vertices(int min_hex_surface_value, int  max_hex_surface_value) {
	hex_surface_index_data_type i, j;
	int n = 0, hex_index = 0;

	hex_surface_object_data_class hex_data = hex_surface_object_data;

	//QMessageBox::information(NULL, "","hex_surface_object_class :: define_vbo_vertices00 :"+QString::number(point_cloud.vertices.size()), QMessageBox::Ok);
	//if (log_panel != NULL) {
	//	//log_display.dialog = true;
	//	log_panel->log_message(log_display, log_message_type_enum_type::debug, "hex_surface_object_class :: define_vbo_vertices00 :"+QString::number(point_cloud.vertices.size()));
	//}

	point_cloud.vertices.clear();

	//if (log_panel != NULL) {
	//	log_panel->log_message(log_display, log_message_type_enum_type::debug, "define_vbo_vertices 00 : " + QString::number(min_hex_surface_value) + " : " + QString::number(max_hex_surface_value));
	//}

	//QMessageBox::information(NULL, "vertices","define_cubic_hex_surface_cloudA "+QString::number(hex_data.hex_surface_matrix_data.size()), QMessageBox::Ok);
	//QMessageBox::information(NULL, "vertices","define_cubic_hex_surface_cloudB "+QString::number(hex_data.hex_surface_object_data[0].size()), QMessageBox::Ok);
	//QMessageBox::information(NULL, "vertices","define_cubic_hex_surface_cloudC "+QString::number(hex_data.hex_surface_object_data[0][0].size()), QMessageBox::Ok);

	hex_surface_index_data_type  dim_x, dim_y;

	dim_y = hex_data.grid_dimension.y;
	//QMessageBox::information(NULL, "vertices","Number_vertices : "+ QString::number(cloud->vertices.size()), QMessageBox::Ok);
	for (j = 0; j < dim_y && n < MAX_HEX_SURFACE_VERTICES; j++) { // y axix integer block  of 32 integer bit nodes
		dim_x = (j + 1) % 2 * hex_data.grid_dimension.x + (j % 2) * (hex_data.grid_dimension.x - 1);
		for (i = 0; i < dim_x && n < MAX_HEX_SURFACE_VERTICES; i++) {// x axis integer
			//if(hex_surface_index > hex_data.hex_surface_matrix_data.size()){
			//	QMessageBox::information(NULL, "hexBS01","hexBS01 :"+ QString::number(i)+" : "+QString::number(j)+" : "+QString::number(k)+" : "+QString::number(hex_surface_index), QMessageBox::Ok);
			//return false;	
			//}
			add_hex_surface_vbo_vertex(hex_index);

			hex_index++;

		}
	}

	//QMessageBox::information(NULL, "","hex_surface_object_class:: define_vbo_vertices05 :"+QString::number(point_cloud.vertices.size()), QMessageBox::Ok);
	//if (log_panel != NULL) {
	//	//log_display.dialog = true;
	//	log_panel->log_message(log_display, log_message_type_enum_type::debug, "hex_surface_object_class :: define_vbo_vertices05 :" + QString::number(point_cloud.vertices.size()));
	//}
			//if (point_cloud.n_vertices() < 1)
			//	return false; // need better handling here
			//else


	update_bounding_area();

	return true;
}

bool hex_surface_object_class::add_hex_surface_vbo_vertex(int index) {
	float x, y, z;

	glm::vec3 vertex;

	glm::vec3                     hex_surface_cartesian_coord;
	hex_surface_data_type         surface_data;
	hex_surface_object_data_class hex_data = hex_surface_object_data;

	surface_data = hex_data.hex_surface_matrix_data[index];

	glm::ivec3 iv = hex_surface_object_data.get_matrix_coordinate(index);

	hex_surface_cartesian_coord = hex_surface_object_data.get_hex_surface_cartesian_coordinate(iv);
	x = hex_surface_object_data.grid_origin.x + hex_surface_cartesian_coord.x;
	y = hex_surface_object_data.grid_origin.y + hex_surface_cartesian_coord.y;
	z = surface_data;

	vertex = { x,y,z };

	point_cloud.add_vertex(vertex);

	return true;
}


int hex_surface_object_class::define_vbo_faces() {
	int x_dim = hex_surface_object_data.grid_dimension.x;
	int y_dim = hex_surface_object_data.grid_dimension.y;

	int number_even_rows = int(y_dim / 2) + y_dim % 2;
	int number_odd_rows = int(y_dim / 2);

	int n = 0;

	//QMessageBox::information(NULL, "","hex_surface_object_class :: define_vbo_faces 00 :"+QString::number(point_cloud.vertices.size()), QMessageBox::Ok);
	//if (log_panel != NULL) {
	//	//log_display.dialog = true;
	//	log_panel->log_message(log_display, log_message_type_enum_type::debug, "hex_surface_object_class :: define_vbo_faces 00 :" + QString::number(point_cloud.vertices.size()));
	//}
	point_cloud.vertices.clear();

	for (int j = 0; j < y_dim; j++) {
		int x_d = x_dim - j % 2;
		for (int i = 0; i < x_d; i++) {

			if (j % 2 == 0 && j > 0) { // have even row
				if (i > 0) {
					add_hex_surface_vbo_vertex(n - x_dim);
					add_hex_surface_vbo_vertex(n);
					add_hex_surface_vbo_vertex(n - 1);

					//stream << "3 " << n - x_dim << " " << n << " " << n - 1 << endl;
					if (i < x_dim - 1) {

						add_hex_surface_vbo_vertex(n - x_dim + 1);
						add_hex_surface_vbo_vertex(n);
						add_hex_surface_vbo_vertex(n - x_dim);

						//stream << "3 " << n - x_dim + 1 << " " << n << " " <<  n - x_dim << endl;
					}
				}
			}

			if (j % 2 != 0) {// have odd row
				if (i > 0) {
					add_hex_surface_vbo_vertex(n - x_dim);
					add_hex_surface_vbo_vertex(n);
					add_hex_surface_vbo_vertex(n - 1);

					//stream << "3 " << n - x_dim << " " << n << " " <<  n - 1 << endl;
				}

				add_hex_surface_vbo_vertex(n - x_dim + 1);
				add_hex_surface_vbo_vertex(n);
				add_hex_surface_vbo_vertex(n - x_dim);

				//stream << "3 " << n - x_dim + 1 << " " << n << " " << n - x_dim << endl;
			}
			n++;
		}
	}

	//QMessageBox::information(NULL, "","hex_surface_object_class :: define_vbo_faces 01 :"+QString::number(point_cloud.vertices.size()), QMessageBox::Ok);
	//if (log_panel != NULL) {
	//	//log_display.dialog = true;
	//	log_panel->log_message(log_display, log_message_type_enum_type::debug, "hex_surface_object_class :: define_vbo_faces 01 :" + QString::number(point_cloud.vertices.size()));
	//}
	return n;
}

glm::vec3 hex_surface_object_class::calculate_normal_vector(glm::vec3& vertex_0, glm::vec3& vertex_1, glm::vec3& vertex_2) {
	glm::vec3 u_vector = vertex_1 - vertex_0;
	glm::vec3 v_vector = vertex_2 - vertex_1;

	return glm::cross(u_vector, v_vector);
}

//This assumes that point_cloud.vertices is ordered as that of the hex_surface_object_data.hex_surface_matrix_data 
//surface grid and that the hex surface vertices for point data has been defined using define_vbo_vertices
bool hex_surface_object_class::define_vbo_point_normals() {
	int x_dim = hex_surface_object_data.grid_dimension.x;
	int y_dim = hex_surface_object_data.grid_dimension.y;

	if (y_dim < 2) {
		// error message here
		return false;
	}

	int number_even_rows = int(y_dim / 2) + y_dim % 2;
	int number_odd_rows = int(y_dim / 2);

	int n = 0;

	glm::vec3 vertex_0, vertex_1, vertex_2;

	for (int j = 0; j < y_dim; j++) {
		int x_d = x_dim - j % 2;
		for (int i = 0; i < x_d; i++) {

			if (j % 2 == 0 && j > 0) { // have even row
				if (i > 0) {
					vertex_0 = point_cloud.vertices.mVertices[n - x_dim];
					vertex_1 = point_cloud.vertices.mVertices[n];
					vertex_2 = point_cloud.vertices.mVertices[n - 1];

					glm::vec3 normal = calculate_normal_vector(vertex_0, vertex_1, vertex_2);

					point_cloud.vertices.mNormals[n - x_dim] = normal;
					point_cloud.vertices.mNormals[n] = normal;
					point_cloud.vertices.mNormals[n - 1] = normal;

					//stream << "3 " << n - x_dim << " " << n << " " << n - 1 << endl;
					if (i < x_dim - 1) {

						vertex_0 = point_cloud.vertices.mVertices[n - x_dim + 1];
						vertex_1 = point_cloud.vertices.mVertices[n];
						vertex_2 = point_cloud.vertices.mVertices[n - x_dim];

						glm::vec3 normal = calculate_normal_vector(vertex_0, vertex_1, vertex_2);

						point_cloud.vertices.mNormals[n - x_dim + 1] = normal;
						point_cloud.vertices.mNormals[n] = normal;
						point_cloud.vertices.mNormals[n - x_dim] = normal;

						//stream << "3 " << n - x_dim + 1 << " " << n << " " <<  n - x_dim << endl;
					}
				}
			}

			if (j % 2 != 0) {// have odd row
				if (i > 0) {
					vertex_0 = point_cloud.vertices.mVertices[n - x_dim];
					vertex_1 = point_cloud.vertices.mVertices[n];
					vertex_2 = point_cloud.vertices.mVertices[n - 1];

					glm::vec3 normal = calculate_normal_vector(vertex_0, vertex_1, vertex_2);

					point_cloud.vertices.mNormals[n - x_dim] = normal;
					point_cloud.vertices.mNormals[n] = normal;
					point_cloud.vertices.mNormals[n - 1] = normal;

					//stream << "3 " << n - x_dim << " " << n << " " <<  n - 1 << endl;
				}

				vertex_0 = point_cloud.vertices.mVertices[n - x_dim + 1];
				vertex_1 = point_cloud.vertices.mVertices[n];
				vertex_2 = point_cloud.vertices.mVertices[n - x_dim];

				glm::vec3 normal = calculate_normal_vector(vertex_0, vertex_1, vertex_2);

				point_cloud.vertices.mNormals[n - x_dim + 1] = normal;
				point_cloud.vertices.mNormals[n] = normal;
				point_cloud.vertices.mNormals[n - x_dim] = normal;

				//stream << "3 " << n - x_dim + 1 << " " << n << " " << n - x_dim << endl;
			}
			n++;
		}
	}

	return true;
}

// This assumes that point_cloud.vertices is ordered as a series of individual triangles
// that has been defined using define_vbo_faces
bool hex_surface_object_class::define_vbo_face_normals() {
	//int number_vertices = point_cloud.vertices.size();
	int number_vertices = point_cloud.vertices.number_of_vertices();

	if (number_vertices < 3) {
		// error message here
		return false;
	}

	int number_triangles = number_vertices / 3;

	for (int i = 0; i < number_triangles; i++) {
		int offset = i * 3;
		glm::vec3 vertex_0 = point_cloud.vertices.mVertices[offset];
		glm::vec3 vertex_1 = point_cloud.vertices.mVertices[offset + 1];
		glm::vec3 vertex_2 = point_cloud.vertices.mVertices[offset + 2];

		glm::vec3 normal = calculate_normal_vector(vertex_0, vertex_1, vertex_2);

		point_cloud.vertices.mNormals[offset] = normal;
		point_cloud.vertices.mNormals[offset + 1] = normal;
		point_cloud.vertices.mNormals[offset + 2] = normal;
	}

	return true;
}

void hex_surface_object_class::update_bounding_area() {
	float min_x = hex_surface_object_data.hex_surface_generator_parameters.x_start;
	float min_y = hex_surface_object_data.hex_surface_generator_parameters.y_start;

	float max_x = hex_surface_object_data.hex_surface_generator_parameters.x_end;
	float max_y = hex_surface_object_data.hex_surface_generator_parameters.y_end;

	bounding_area.update_limits(glm::vec3(min_x, min_y, 0.0), glm::vec3(max_x, max_y, 0.0));
}

/*		//*****
	void display_parameters(scene_manager_class *scene_manager = NULL, log_panel_class *log_panel = NULL, id_type current_selected_object_id = -1) {
printf("parameter_panel_class :: display_hex parameters 0000 %i \n", current_selected_object_id);

		hex_surface_parameters_widget_class hex_surface_parameters_widget;

		hex_surface_parameters_widget.scene_manager = scene_manager;
		hex_surface_parameters_widget.log_panel     = log_panel;

		hex_surface_parameters_widget.display_parameters_widget(globalc::get_current_selected_entity_id());

	}
*/
//*****