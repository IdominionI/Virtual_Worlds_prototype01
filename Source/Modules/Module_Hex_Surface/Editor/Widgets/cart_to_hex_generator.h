#pragma once
#include "../../Hex_surface_object/hex_surface_object.h"
#include "../../Render/hex_surface_render.h"

class cart_to_hex_widget_class {
public:
	log_panel_class* log_panel = NULL;

	id_type                   current_selected_entity_id = -1;  // entity id of the selected entity to display/modify
	hex_surface_object_class *hex_surface_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
	scene_manager_class      *scene_manager = NULL;

	void display();
	void define_cartesian_data();
	void import_ply_file_to_cartesian_array();
	void generate_cartesian_array();
	glm::vec3 xy_function();
	glm::vec3 rnd_circle();
	glm::vec3 random_xy(float min_x, float max_x, float min_y, float max_y, float scale = 1.0f);
	void define_generation_parameters();
	void define_default_hex_limits();
	void cartesian_to_voxel(bool notification = true);

private:
	float ctov_scale = 1.0f; // testing only

	int  number_of_points = 1000;
	bool display_average_z_value  = false;

	int random_type     = 0;
	int generation_type = 1;

	struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };

	string_type   ply_file_pathname = "";
	string_type   ply_file_name = "...";

	float min_x = -10.0f, max_x = 10.0f;
	float min_y = -10.0f, max_y = 10.0f;

	float hex_size        = 1.0f;
	float hex_scale_value = 1.0f;
	float min_vscale = 0.001f, max_vscale = 1.000f;

	int min_data_value = 1, max_data_value = 100;

	hex_surface_render_class   hex_surface_render;

	struct hex_cartesian_point_data_struct_type {
		float min_x = 0.0f, max_x = 0.0f;
		float min_y = 0.0f, max_y = 0.0f;
		float min_z = 0.0f, max_z = 0.0f;

		std::vector<glm::vec3> coordinates;

		void add_coordinate(glm::vec3 coordinate) {
			if (coordinate.x < min_x) min_x = coordinate.x;
			if (coordinate.x > max_x) max_x = coordinate.x;
			if (coordinate.y < min_y) min_y = coordinate.y;
			if (coordinate.y > max_y) max_y = coordinate.y;
			if (coordinate.z < min_z) min_z = coordinate.z;
			if (coordinate.z > max_z) max_z = coordinate.z;

			coordinates.push_back(coordinate);
		}

		void reset() {
			coordinates.clear();
			coordinates.shrink_to_fit();
			min_x = 0.0f; max_x = 0.0f;
			min_y = 0.0f; max_y = 0.0f;
			min_z = 0.0f; max_z = 0.0f;
		}
	};

	hex_cartesian_point_data_struct_type cartesian_point_data;

	void get_ply_file();
	bool import_file(const std::string& filepath);

};