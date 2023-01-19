#pragma once

#include <Source/Editor/Main_Window/Panels/log_panel.h>

#include <Source/Graphics_Engine/Scene/Scene_objects/camera_object.h>

#include <Source/Editor/Object/object_basis.h>
#include <Source/Graphics_Engine/Geometry/point_cloud.h>

#include "hex_surface_data_storage.h"

#include <Source/Graphics_Engine/Scene/Scene_objects/bounding_area_object.h>

enum class hex_surface_vertex_order_enum { none, face, points, line };

/*
	hex_surface_object_class

	C++ class that represents and manages a hex surface data object to be displayed and
	processed in and by the Virtual Worlds application.

*/

class hex_surface_object_class : public object_basis_class {

public:
	log_panel_class                  *log_panel = NULL;

	point_cloud3D_class               point_cloud;
	std::vector<int>                  vertex_indicies;
	hex_surface_object_data_class     hex_surface_object_data;
	camera_object_class              *camera = NULL;

	int number_faces = 0;
	
	hex_surface_vertex_order_enum  point_cloud_order = hex_surface_vertex_order_enum::none;

	bounding_area_class bounding_area;

	bool display_hex_surface_object_as_points = false;

	bool  animate_automata = false;

	glm::uvec4 color                          = {1.0,1.0,1.0,1.0};

	float min_surface_display_value, max_surface_display_value;

	void clear_shader_variables();
	bool automata_rule_within_frame(int frame);
	bool define_vbo_vertices(int min_hex_surface_value, int  max_hex_surface_value);
	bool add_hex_surface_vbo_vertex(int index);
	int  define_vbo_faces();
	glm::vec3 calculate_normal_vector(glm::vec3& vertex_0, glm::vec3& vertex_1, glm::vec3& vertex_2);
	bool define_vbo_point_normals();
	bool define_vbo_face_normals();
	void update_bounding_area();

};
