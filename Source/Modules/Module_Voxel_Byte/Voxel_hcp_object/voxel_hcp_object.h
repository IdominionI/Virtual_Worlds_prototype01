#pragma once

#include "voxel_data_storage.h"
//#include "../Editor/Widgets/parameters_widget.h" // ****

#include <Source/Editor/Object/object_basis.h>
#include <Source/Graphics_Engine/Geometry/point_cloud.h>
#include <Source/Graphics_Engine/Scene/Scene_objects/bounding_volume_object.h>


/*
	voxel_hcp_object_class

	C++ class that represents and manages a HCP voxel volume data object
	to be displayed and processed in and by the Virtual Worlds application.
*/


class voxel_hcp_object_class : public object_basis_class {
public:
	point_cloud3DD_class    point_cloud;
	voxel_object_data_class voxel_object_data;

	bounding_volume_class bounding_volume;

	~voxel_hcp_object_class(){
		delete bounding_volume.geometry;
	}

	//bool  active_object = false;
	bool  display_voxel_object_as_points = false;

	bool  animate_automata = false;

	bool update_buffer = false;

	float min_surface_display_value, max_surface_display_value;

	void clear_shader_variables();
	bool automata_rule_within_frame(int frame);
	bool define_vbo_vertices(int min_voxel_value, int  max_voxel_value);
	void update_bounding_volume();

};