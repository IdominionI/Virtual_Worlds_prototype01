#include "voxel_hcp_object.h"

void voxel_hcp_object_class::clear_shader_variables() {
	voxel_object_data.shader_parameters.variables.clear();
	voxel_object_data.shader_parameters.variables.shrink_to_fit();
	voxel_object_data.shader_parameters.int_variables.clear();
	voxel_object_data.shader_parameters.int_variables.shrink_to_fit();
	voxel_object_data.shader_parameters.bool_variables.clear();
	voxel_object_data.shader_parameters.bool_variables.shrink_to_fit();
	// other variables added below
}

bool voxel_hcp_object_class::automata_rule_within_frame(int frame) {
	bool is_within_animation_frame = false;
	for (voxel_hcp_automata_byte_rule_struct_type autmata_rule : voxel_object_data.voxel_hcp_automata_byte_rules) {
		if (frame >= autmata_rule.start_step && frame <= autmata_rule.end_step) is_within_animation_frame = true;
	}

	return is_within_animation_frame;
}

bool voxel_hcp_object_class::define_vbo_vertices(int min_voxel_value, int  max_voxel_value) {
	index_data_type i, j, k;
	int n = 0, voxel_index = 0;
	float x, y, z;

	voxel_object_data_class voxel_data = voxel_object_data;
	voxel_data_type         volume_data;
	voxel_element_data_type volume_data_value;

	glm::vec4  vertex;

	point_cloud.vertices.clear();

	//printf("voxel_hcp_object_class : define_vbo_vertices 000 : %i\n", voxel_data.voxel_matrix_data.size());
	//printf("voxel_hcp_object_class : define_vbo_vertices 111 : %f %f %f\n", voxel_object_data.matrix_origin.x, voxel_object_data.matrix_origin.y, voxel_object_data.matrix_origin.z);

	//QMessageBox::information(NULL, "vertices","define_cubic_voxel_cloudA "+QString::number(voxel_data.voxel_matrix_data.size()), QMessageBox::Ok);
	//QMessageBox::information(NULL, "vertices","define_cubic_voxel_cloudB "+QString::number(voxel_data.voxel_object_data[0].size()), QMessageBox::Ok);
	//QMessageBox::information(NULL, "vertices","define_cubic_voxel_cloudC "+QString::number(voxel_data.voxel_object_data[0][0].size()), QMessageBox::Ok);

			//float sqrt3 = sqrt(3.0),third = 1.0/3.0, z_mult = 2.0*sqrt(6.0)/3.0, sqrt3_2=sqrt(1.5);// This original lagacy code left here until found to be totaly uneccesary to keep
	bool even_z_level;
	index_data_type  dim_x, dim_y;

	for (k = 0; k < voxel_data.matrix_dimension.z && n < MAX_VOXEL_VERTICES; k++) {
		if (k % 2 == 0)
			even_z_level = true;
		else
			even_z_level = false;

		dim_y = ((k + 1) % 2) * voxel_data.matrix_dimension.y + (k % 2) * (voxel_data.matrix_dimension.y - 1);
		//QMessageBox::information(NULL, "vertices","Number_vertices : "+ QString::number(cloud->vertices.size()), QMessageBox::Ok);
		for (j = 0; j < dim_y && n < MAX_VOXEL_VERTICES; j++) { // y axix integer block  of 32 integer bit nodes
			if (even_z_level) {
				dim_x = (j + 1) % 2 * voxel_data.matrix_dimension.x + (j % 2) * (voxel_data.matrix_dimension.x - 1);
			}
			else {
				dim_x = (j + 1) % 2 * (voxel_data.matrix_dimension.x - 1) + (j % 2) * voxel_data.matrix_dimension.x;
			}

			for (i = 0; i < dim_x && n < MAX_VOXEL_VERTICES; i++) {// x axis integer
				//if(voxel_index > voxel_data.voxel_matrix_data.size()){
				//	QMessageBox::information(NULL, "voxelBS01","voxelBS01 :"+ QString::number(i)+" : "+QString::number(j)+" : "+QString::number(k)+" : "+QString::number(voxel_index), QMessageBox::Ok);
				//return false;	
				//}

				volume_data = voxel_data.voxel_matrix_data[voxel_index];
				volume_data_value = voxel_data.extract_voxel_data_element_value(data_storage_type_enum::value, voxel_index);

				//if (log_widget != NULL) {
				//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "voxel_hcp_object_class define_vbo_vertices : " + QString::number(volume_data) + " : " + QString::number(volume_data_value));
				//}

				if (volume_data_value > INVALID_VOXEL_VALUE && volume_data_value <= MAX_VOXEL_VALUE) {
					// HCP voxel creation

					glm::vec3 voxel_cartesian_coord = voxel_object_data.get_voxel_cartesian_coordinate({ i,j,k });

					x = voxel_object_data.matrix_origin.x + voxel_cartesian_coord.x;
					y = voxel_object_data.matrix_origin.y + voxel_cartesian_coord.y;
					z = voxel_object_data.matrix_origin.z + voxel_cartesian_coord.z;

					//QMessageBox::information(NULL, "Generate Voxel function", "define_vbo_vertices 00:" + QString::number(x)+" : "+ QString::number(y) + " : " + QString::number(z), QMessageBox::Ok);
					//if (log_widget != NULL) {
					//	log_widget->log_message(log_display, log_message_type_enum_type::debug, "define_vbo_vertices 00 : " + QString::number(x) + " : " + QString::number(y) + " : " + QString::number(z) + +" : " + QString::number(vertex->value));
					//}

					voxel_element_data_type lower_range = voxel_element_data_type(min_voxel_value), upper_range = voxel_element_data_type(max_voxel_value);

					if (lower_range == INVALID_VOXEL_VALUE) lower_range = INVALID_VOXEL_VALUE + 1;
					if (n < MAX_VOXEL_VERTICES && volume_data_value >= lower_range && volume_data_value <= upper_range) {

						//vertex.mPos   = { x,y,z,(float)volume_data };// volume data passed as a float value stored in a vec4 .w variable
						vertex = { x,y,z,(float)volume_data };// volume data passed as a float value stored in a vec4 .w variable // ******

						//printf("define_vbo_vertices : define_vbo_vertices 2222 %f : %f : % f :: %i\n", x,y,z, volume_data_value);
						point_cloud.add_vertex(vertex);
						n++;
					}
				}

				//if(vbo_vertices.size()<1)
				//QMessageBox::information(NULL, "voxelBS03", "vbo_vertices.size()<1"+ QString::number(n)+" : "+ QString::number(offset)+" : "+QString::number(x)+"::"+QString::number(y)+"::"+QString::number(z), QMessageBox::Ok);
				//QMessageBox::information(NULL, "voxelBS03", "voxelBS03C :"+ QString::number(n)+" : "+ QString::number(offset)+" : "+QString::number(vbo_vertices[0].x())+"::"+QString::number(vbo_vertices[0].y())+"::"+QString::number(vbo_vertices[0].z()), QMessageBox::Ok);
				//QMessageBox::information(NULL, "voxelBS04","voxelBS04 :"+ QString::number(offset)+" : "+QString::number(cloud->vertices[n].world_location.y())+" : "+QString::number(cloud->vertices[n].world_location.z()), QMessageBox::Ok);

				voxel_index++;

			}
		}
	}

	//printf("voxel_hcp_object_class : define_vbo_vertices 111 : %i\n", point_cloud.vertices.size());
	//QMessageBox::information(NULL, "define_vbo_vertices","define_vbo_vertices05 :"+QString::number(i)+" : "+QString::number(j)+" : "+QString::number(k)+" : "+QString::number(voxel_index)+" : "+QString::number(vertices.size()), QMessageBox::Ok);

	update_bounding_volume();

	return true;
}

void voxel_hcp_object_class::update_bounding_volume() {
	//float min_x = voxel_object_data.voxel_generator_parameters.x_start;
	//float min_y = voxel_object_data.voxel_generator_parameters.y_start;
	//float min_z = voxel_object_data.voxel_generator_parameters.z_start;

	//float max_x = voxel_object_data.voxel_generator_parameters.x_end;
	//float max_y = voxel_object_data.voxel_generator_parameters.y_end;
	//float max_z = voxel_object_data.voxel_generator_parameters.z_end;

	float min_x = voxel_object_data.matrix_origin.x;
	float min_y = voxel_object_data.matrix_origin.y;
	float min_z = voxel_object_data.matrix_origin.z;

	float max_x = voxel_object_data.matrix_origin.x + (voxel_object_data.voxel_generator_parameters.x_end - voxel_object_data.voxel_generator_parameters.x_start);
	float max_y = voxel_object_data.matrix_origin.y + (voxel_object_data.voxel_generator_parameters.y_end - voxel_object_data.voxel_generator_parameters.y_start);
	float max_z = voxel_object_data.matrix_origin.z + (voxel_object_data.voxel_generator_parameters.z_end - voxel_object_data.voxel_generator_parameters.z_start);

	bounding_volume.update_limits(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, max_y, max_z));
}

//*****
/*	void display_parameters(scene_manager_class* scene_manager = NULL, log_panel_class* log_panel = NULL, id_type current_selected_object_id = -1) {
//printf("parameter_panel_class :: display_voxel_hcp_parameters_widget 0000 %i \n", current_selected_object_id);

		voxel_hcp_parameters_widget_class   voxel_hcp_parameters_widget;

		voxel_hcp_parameters_widget.scene_manager = scene_manager;
		voxel_hcp_parameters_widget.log_panel = log_panel;
//printf("parameter_panel_class :: display_voxel_hcp_parameters_widget 0000 %i \n", *current_selected_object_id);

		voxel_hcp_parameters_widget.display_parameters_widget(globalc::get_current_selected_entity_id());

	}

*/
